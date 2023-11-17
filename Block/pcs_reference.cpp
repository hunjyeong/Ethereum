#include <mcl/bls12_381.hpp>
#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <malloc.h>
#include <mcl/ntt.hpp>
#include <chrono>
#include <mcl/lagrange.hpp>
#include "primitiveroots.hpp"
#include "polyonfr.cpp"

struct pedersen{

    struct pp{
        vector<G1> GG;  // generator (g0, g1, ... , gd) 선언
        G1 H;  // generator h 선언
    }pp;

    pedersen(){
        this->pp.GG.clear();
        this->pp.H.clear();
    }

    void init(uint32_t d){

        this->pp.GG.resize(d);  // size d로 초기화
        string tmp;
        for(uint32_t i=0;i<d;i++){
            hashAndMapToG1(this->pp.GG[i],"g"+to_string(i));  // index처럼 g0, g1, g2 ... g(d-1)까지 generator 생성 -> GG[i]에 저장?
        }
        hashAndMapToG1(this->pp.H,"H");  // H에 generator h 저장?

    }

    G1 commit(FrVec a,bool zk=false){  // commitment 함수 a = p(x)
        this->init(a.size());
        G1 res;
        G1::mulVec(res,&this->pp.GG[0], &a[0], a.size());  // GG[i]^a[i] 연산 --> res = (g_0^a_0)(g_1^a_1)...(g_n-1^a_d-1)
        if(zk){  // zk = true --> H^r 연산
            Fr r;
            r.setByCSPRNG();
            res=res+(this->pp.H)*r;  // res = res + H*r = (g_0^a_0)(g_1^a_1)...(g_n-1^a_d-1)(H^r)
        }
        return res;
    }

};

struct Pedersen_PCS{  // pedersen commitment scheme

    public:
    pedersen pedersen1;
    G1 P;  // commitment P = com(a) = com(p(x))
    Fr x;  // evaluation point x
    Fr v;  // evaluation value v
    uint32_t d;

    struct CRS{ // Common Reference String --> Prover, Verifier 알고 있는 값

        G1 Pd; // P' 선언
        G1 U; // random oracle로부터 뽑힌 random U 선언
        vector<G1> L;
        vector<G1> R;
        FrVec B; // B = (1, x, x^2, ..., x^d-1) 선언 
        G1 G; // G(0)
        Fr b; //b(0)

    }CRS;

    void init(FrVec a){

        uint32_t N = nextPowOf2(a.size());  // N = 2^k (k: round 수)
        a.resize(N,0);  // N으로 resize하고 빈칸은 0으로 패딩

        this->pedersen1.init(a.size());
        this->P=pedersen1.commit(a);   // P = com(a) = (g_0^a_0)(g_1^a_1)...(g_n-1^a_d-1)(h^r) //a = p(x)
        hashAndMapToG1(this->CRS.U,P.getStr());   // com(a) --> random oracle --> U 뽑음
        this->x.setHashOf(P.getStr());   // P로부터 evaluation point x 뽑음
        evaluatePolynomial<Fr,Fr>(this->v,&a[0],a.size(),x);   // evaluation value v = p(x) 
        this->CRS.Pd = P+(this->CRS.U*v);   // P' = P + [v]*U
        this->CRS.B.resize(a.size());   // B vector도 a 벡터 크기와 맞게 resize

        for(uint32_t i=0;i<a.size();i++){

            Fr::pow(this->CRS.B[i], x, i);  // B = (1, x, x^2, ..., x^d-1) / x : evaluation point
        }

    }

    void IPA_prover(vector<G1> GG, FrVec a, FrVec B){   //inner product argument 함수

        if(a.size()==1){  // 크기가 1 --> stop?

            this->CRS.b = B[0];  // b = 1
            this->CRS.G = GG[0];  // G = g_0

        }else{

            FrVec a_lo(a.begin(),a.begin()+a.size()/2);  // a_lo = (a_0, a_1, ..., a_(d-1/2))
            FrVec a_hi(a.begin()+a.size()/2, a.end());  // a_hi = (a_(d-1/2)+1, ..., a_d-1)

            FrVec b_lo(B.begin(),B.begin()+B.size()/2);  // b_lo = (1, x, x^2, ..., x^(d-1/2))
            FrVec b_hi(B.begin()+B.size()/2,B.end());  // b_hi = (x^(d-1/2)+1, ..., x^d)

            vector<G1> g_lo(GG.begin(),GG.begin()+GG.size()/2);  // g_lo = (G0, G1, ..., G(d-1/2))
            vector<G1> g_hi(GG.begin()+GG.size()/2,GG.end());  // g_hi = (G(d-1/2 +1), ..., G(d-1))

            G1 l;
            G1::mulVec(l,&g_hi[0],&a_lo[0],a_lo.size());  // l = a_lo[0] * g_hi[0]
            this->CRS.L.insert(this->CRS.L.begin(),l);
            G1 r;
            G1::mulVec(r,&g_lo[0],&a_hi[0],a_hi.size());  // r = a_hi[0] * g_lo[0]
            this->CRS.R.insert(this->CRS.R.begin(),r);

            Fr u;
            u.setHashOf(l.getStr()+r.getStr());  // uk : challenge 값 

            FrVec tmp_u={u};  // uk
            FrVec tmp_invu={1/u};  // uk^(-1)

            FrVec a_new;
            FrVec b_new;
            vector<G1> g_new;

            a_new = PolyAdd(PolyMul(tmp_invu,a_hi),PolyMul(tmp_u,a_lo));  // a_new = (a_lo * uk) + (a_hi * uk^(-1)) / a 크기 반으로 줄임 : a(k-1) = (a_lo(k) * uk) + (a_hi(k) * uk^(-1))
            b_new = PolyAdd(PolyMul(tmp_invu,b_lo),PolyMul(tmp_u,b_hi));  // b_new = (b_lo * uk^(-1)) + (b_hi * uk) / b 크기 반으로 줄임 : b(k-1)
           
            G1 tmp_g;
            for (int i = 0; i<GG.size()/2; i++){

                tmp_g = g_lo[i] * (1/u) + g_hi[i] * u;  // tmg_g(k-1) = g_lo(k)*uk^(-1) + g_hi(k)*uk // G 반으로 줄임
                g_new.push_back(tmp_g);  // g_new = (g_lo[0]*(1/u) + g_hi[0]*u)(g_lo[1]*(1/u) + g_hi[1]]*u) ...

            }

            IPA_prover(g_new, a_new, b_new);  // G(k-1), a(k-1), b(k-1) --> 다음 round의 input 값으로 줌

        }
    }
};

int main() {

    /*FrVec ap = {1,2,3,4,5,6};
    FrVec Fb;
    Fb.clear();
    vector<G1> pG;
    pG.clear();

    pedersen ped;
    Pedersen_PCS pcs;

    ped.commit(ap);  // pedersen commit(FrVec a,bool zk=false)
    pG = ped.Ggg(ap); // vector<G1> Ggg(FrVec a)
    Fb = pcs.BB(ap);  // FrVec BB(FrVec a)
    pcs.IPA_prover(pG, ap, Fb);  // Pedersen_PCS IPA_prover(vector<G1> GG, FrVec a, FrVec B)
    */


}

