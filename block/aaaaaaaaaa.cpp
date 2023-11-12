#include <algorithm>
#include <vector>
#include <string>
#include <string.h>
#include <numeric>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>    //crypto.h, evp.h 차이


//[i_0, i_1 ... i_n, value]

def update(node,path,value){
    if path == '':
        curnode = db.get(node) if node else [ NULL ] * 17
        newnode = curnode.copy()
        newnode[-1] = value
    else:
        curnode = db.get(node) if node else [ NULL ] * 17 
        newnode = curnode.copy()
        newindex = update(curnode[path[0]],path[1:],value)
        newnode[path[0]] = newindex
    db.put(hash(newnode),newnode)
    return hash(newnode)
}

def delete(node,path){
    if node is NULL:
        return NULL
    else:
        curnode = db.get(node)
        newnode = curnode.copy()
        if path == '':
            newnode[-1] = NULL
        else:
            newindex = delete(curnode[path[0]],path[1:])
            newnode[path[0]] = newindex

        if all(x is NULL for x in newnode):
            return NULL
        else:
            db.put(hash(newnode),newnode)
            return hash(newnode)

}

/*
hex char    bits    |    node type partial     path length
----------------------------------------------------------
   0        0000    |       extension              even
   1        0001    |       extension              odd
   2        0010    |   terminating (leaf)         even
   3        0011    |   terminating (leaf)         odd
*/

def compact_encode(hexarray){
    term = 1 if hexarray[-1] == 16 else 0
    if term: hexarray = hexarray[:-1]
    oddlen = len(hexarray) % 2
    flags = 2 * term + oddlen
    if oddlen:  // 길이 홀수
        hexarray = [flags] + hexarray   //1, 3
    else:       // 길이 짝수
        hexarray = [flags] + [0] + hexarray     //00, 02
    // hexarray now has an even length whose first nibble is the flags.
    o = ''
    for i in range(0,len(hexarray),2):
        o += chr(16 * hexarray[i] + hexarray[i+1])  //2개씩 묶는 과정인듯?
    return o
}


/*
> [ 1, 2, 3, 4, 5, ...]
'11 23 45'
> [ 0, 1, 2, 3, 4, 5, ...]
'00 01 23 45'
> [ 0, f, 1, c, b, 8, 10]
'20 0f 1c b8'
> [ f, 1, c, b, 8, 10]
'3f 1c b8'
*/

def get_helper(node,path){
    if path == []: return node
    if node = '': return ''
    curnode = rlp.decode(node if len(node) < 32 else db.get(node))
    if len(curnode) == 2:
        (k2, v2) = curnode
        k2 = compact_decode(k2)
        if k2 == path[:len(k2)]:
            return get(v2, path[len(k2):])
        else:
            return ''
    elif len(curnode) == 17:
        return get_helper(curnode[path[0]],path[1:])
}

def get(node,path){
    path2 = []
    for i in range(len(path)):
        path2.push(int(ord(path[i]) / 16))
        path2.push(ord(path[i]) % 16)
    path2.push(16)
    return get_helper(node,path2)
}

/*
<64 6f> : 'verb'
<64 6f 67> : 'puppy'
<64 6f 67 65> : 'coin'
<68 6f 72 73 65> : 'stallion'
*/

/*
rootHash: [ <16>, hashA ]
hashA:    [ <>, <>, <>, <>, hashB, <>, <>, <>, [ <20 6f 72 73 65>, 'stallion' ], <>, <>, <>, <>, <>, <>, <>, <> ]
hashB:    [ <00 6f>, hashD ]
hashD:    [ <>, <>, <>, <>, <>, <>, hashE, <>, <>, <>, <>, <>, <>, <>, <>, <>, 'verb' ]
hashE:    [ <17>, [ <>, <>, <>, <>, <>, <>, [ <35>, 'coin' ], <>, <>, <>, <>, <>, <>, <>, <>, <>, 'puppy' ] ]
*/




