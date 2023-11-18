#include <mcl/bls12_381.hpp>
#include <iostream>
#include <limits>
#include <climits>
#include <bitset>
#include <cmath>
#include "primitiveroots.hpp"
#include <cstdlib>
#include <vector>
#include "polyonfr.cpp"



using namespace std;
using namespace mcl::bn;
using namespace mcl;


class PCS {

public:
    //초기화 및 committer
    vector<G1> pp; // generator가 될 pp 선언 
    Fr eval_pt; // evaluation point 선언
    uint32_t dd; // deg받을 인수 선언
    G1 C; // PCS C 선언
    Fr r;
    string buf;

    PCS(uint32_t d){  // pp 초기화 함수 / d는 main 함수에서 입력으로 넣어줌
        dd=d;
        pp.resize(d+1); // 벡터의 크기 재할당
        for(uint32_t i=0;i<d;i++){
            buf = "g"+to_string(i); // index처럼 g0, g1, g2 ... g(d-1)까지 generator 생성 
            hashAndMapToG1(pp[i],"buf"); // generator buf --> pp[0], pp[1], ... pp[d-1]에 저장?
        }
        hashAndMapToG1(pp[d],"h"); // pp[d]번째에 generator h 저장?
    }


    void committer(Fr *pt1, bool zk = true){ // commit하는 함수, commit할 message 값을 포인터로 받음
        
        if(zk== true){
            r.setByCSPRNG(); //"random" value for r

        }else{
            r =0; 
        }

        G1 gx ; //g^x값 저장할 gx 선언
        gx.clear(); // gx 초기화 ? --> size 0?

        for(int i=0; i<this->dd; i++){
            gx += this->pp[i] * pt1[i]; // g^x 연산
        }
        G1 hr = this->pp[this->dd] * r; // h^r 연산
    
        this->C = gx + hr; // Com = g^x * h^r (mod p)--> x*g + r*h 대수 연산

        cout << "com = " << this ->C << endl;

    }

    struct CRS{
        vector<G1> pp;
        vector<G1> b;
        vector<G1> left;
        vector<G1> right;
        uint32_t dd; //put the size of generator length
        G1 u; 
        G1 P;

        string buf;
        void crs_gen(uint32_t d){
            dd=d;
            pp.resize(d);
            for(uint32_t i=0;i<d;i++){
                buf = "g"+to_string(i);
                hashAndMapToG1(pp[i], "buf");
            }

            //p : PCS 만들어야함

            //u fixed? random? later / 픽스할 것인지? 랜덤으로 주는지?
            //b : random z? later / b = <1, Z, Z^2, ..., Z^(n-1)> random으로 Z 어떻게 줄까?
                

        }
        //not include h, only one generator g

        void crs_update(Fr chalval){ // challenger value 받아서 계산하는 함수
            Fr re_chalval;

            re_chalval = 1/chalval; //inverse operation --> inverse 함수명 무엇인지? lib 확인 필요 --> 잘 모르겠음

            for(int i=0; i < this->dd/2; i++){
                chalval * pp[i] + re_chalval * pp[(this->dd / 2) + i];
                //change pp[i] and remove the half of pp 
            };

            dd = pp.size();
        }

        //P -> later, left&right of each round are stored in vector

    };


    //IPA prover
    vector<G1> left; //L
    vector<G1> right; //R
    G1 u;
    G1 P;
    G1 U; // random group
    //Fr ui; // evaluation point
    G1 Pd; // P'
    Fr x; // B벡터 만드는 x
    Fr v; // evaluation v = <a, b>
    vector<Fr> B; //B벡터

    void prover(FrVec a){

        Fr* pt1 =&a[0];
        uint32_t as = nextPowOf2(a.size());
        a.resize(as,0);

        P = (this->C);
        string ur = P.getStr();

        hashAndMapToG1(U, ur);
        
        x.setHashOf(ur);

        //poly B vector

        B.resize(a.size());
    
        B[0] = 1;
        for (int i = 0; i<a.size(); i++){
            Fr::pow(B[i], x, i);
            this->v += pt1[i] * B[i];
        }

        Pd = P+U*(this->v);

    }

    vector<Fr> al; 
    Fr al_bh, ah_bl;
    G1 L_ag, R_ag;
    void IPA(FrVec a){
        Fr* pt1 =&a[0];
        uint32_t as = nextPowOf2(a.size());
        a.resize(as,0);
        B.resize(a.size());

        FrVec a_lo(a.begin(),a.begin()+a.size()/2);
        FrVec a_hi(a.begin()+a.size()/2,a.end());

        FrVec b_lo(B.begin(),B.begin()+B.size()/2);
        FrVec b_hi(B.begin()+B.size()/2,B.end());

        left.resize(log2(this->B.size()));
        right.resize(log2(this->B.size()));

    
        for(int i=0; i<a.size()/2;i++){
            L_ag += (this->pp[(this->dd /2)+ 1 + i]) * a_lo[i];
            al_bh += (this->U)*(a_lo[i]*b_hi[i]);
        }
        left[0] = L_ag+(this->U)*al_bh;

        for(int i=0; i<a.size()/2;i++){
            R_ag += (this->pp[i]) * a_hi[i];
            ah_bl += (this->U)*(a_hi[i]*b_lo[i]);
        }
        right[0] = R_ag+(this->U)*ah_bl;

        //FrVec gg(this->);
    
    }

    void verifier(){
        
    }


    /*struct IPA{

    public:
        vector<G1> left; //L
        vector<G1> right; //R
        //G1 P;
        G1 U; // random group
        //Fr x; // evaluation point
        Fr v; // evaluation v = <a, b>
        G1 Pd; // P'

        IPA(committer )

        void prover(G1 P, Fr v){

            this->P = committer();
            
            string sp = P.getStr();
            hashAndMapToG1(U, sp);
            
            Pd = P + (v*U);

        }

    };*/


};

struct CRS {  //CRS함수

    vector<G1> pp;
    vector<G1> b;
    vector<G1> left;
    vector<G1> right;
    uint32_t dd; //put the size of generator length
    G1 u; 
    G1 P;

    string buf;
    void crs_gen(uint32_t d){
        dd=d;
        pp.resize(d);
        for(uint32_t i=0;i<d;i++){
            buf = "g"+to_string(i);
            hashAndMapToG1(pp[i], "buf");
        }

        //p : PCS 만들어야함

        //u fixed? random? later / 픽스할 것인지? 랜덤으로 주는지?
        //b : random z? later / b = <1, Z, Z^2, ..., Z^(n-1)> random으로 Z 어떻게 줄까?
        

    }
    //not include h, only one generator g

    void crs_update(Fr chalval){ // challenger value 받아서 계산하는 함수
        Fr re_chalval;

        re_chalval = 1/chalval; //inverse operation --> inverse 함수명 무엇인지? lib 확인 필요 --> 잘 모르겠음

        for(int i=0; i < this->dd/2; i++){
            chalval * pp[i] + re_chalval * pp[(this->dd / 2) + i];
            //change pp[i] and remove the half of pp 
        };

        dd = pp.size();
    }

    //P -> later, left&right of each round are stored in vector

};

/*class prover {

private:
    vector<Fr> a;
    uint32_t len; //length of a

public:
    G1 left; //identity later
    G1 right; //identity later
    
    void lr_comput(CRS crs){
        len = a.size();

        for(int i=0;i < crs.dd/2;i++){
            left += crs.pp[(crs.dd / 2) + i] * a[i];
            right += crs.pp[i] * a[(len/2) + i];
        }
        
        left += crs.u * ;//<a-1,b1> --> 수정 필요
        right += crs.u * ;//<a1,b-1> --> 수정 필요

    }

    void wit_update(Fr chalval){
        Fr re_chalval;

        re_chalval = chalval.inv(); //inverse operation later

        for(int i=0; i < len/2; i++){
            chalval * a[i] + re_chalval * a[(len / 2) + i];
            //change a[i] and remove the half of a later
        };

    }

};*/



int main() {

    initPairing(mcl::BLS12_381); // 나중에 pairing 없는 타원곡선으로 바꿀 계획
   
    // ex
    PCS co(4); //deg : 4
    FrVec y = {1,2,3,4}; // y = 1 + 2x + 3x^2 + 4X^3
    Fr *yy = &y[0]; 
    co.committer(yy); // PCS할 message yy 값 인수로 넣음



    //cout << "g = " << g << endl;
    //cout << "h = " << h << endl;
    //cout << "x = " << *pt1 << endl;
    //cout << "r = " << r << endl;
}
