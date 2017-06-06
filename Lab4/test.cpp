/*
 * test.c
 * Zhaonian Zou
 * Harbin Institute of Technology
 * Jun 22, 2011
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <set>
#include <queue>

extern "C"{
    #include "extmem.h"
}

#define blkSize     64  //块的大小
#define bufSize     520 //缓冲区的大小
#define numAllBlk   8   //缓冲区中最多存储的磁盘块数
#define tupleBlk    7
#define allBlk      400  //一共的磁盘块数
using namespace std;

struct R{
    int a;
    int b;
};
struct S{
    int c;
    int d;
};
struct RS{
    int a;
    int b;
    int d;
};
int MAX;
struct hajo{
    int anum[256];
    int bnum[256];
};



Buffer db;
unsigned char* blk;
unsigned int blkAddr[400];
unsigned int baddr[224];
//block 0-47 存储生成的随机数据
//block 48，49

void linear_search()//R.A=40
{
    R rs[112];
    for(int i=0;i<7;i++)
    {
        rs[i].a = 0;
        rs[i].b = 0;
    }
    int cnt = 0;
    for(int j=0;j<16;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            int a = *(blk + 8*i );
            int b = *(blk + 8*i + 4);
            if(a==40)
            {
                rs[cnt].a = a;
                rs[cnt].b = b;
                cnt++;
            }
        }
    }
    freeBlockInBuffer(blk,&db);

    blk = getNewBlockInBuffer(&db);
    for(int i = 0;i < 7;i++){
        *(blk+8*i) = rs[i].a;
        *(blk+8*i+4) =rs[i].b;
    }
    if (writeBlockToDisk(blk, blkAddr[50],&db) != 0) return ;
    freeBlockInBuffer(blk,&db);


    cout << "线性搜索的结果是：" << endl;
    for(int i = 0;i < cnt;i++){
        cout << rs[i].a << " " << rs[i].b << endl;
    }
    cout << "写在第50个磁盘块中。";
    cout <<endl;
}



int Merge(unsigned int left, int ln, unsigned int right, int rn, Buffer *buf, int addr)
{
	int l = 0, r = 0;
	int offset = 0, i;
	unsigned int tmpaddr = left + addr;
	unsigned int *blk1 = NULL, *blk2 = NULL;
	while (l<ln || r<rn)
	{
		if (l == ln)
			*blk1 = MAX;
		else
			blk1 = (unsigned int *)readBlockFromDisk(left + l, buf);
		if (r == rn)
			*blk2 = MAX;
		else
			blk2 = (unsigned int *)readBlockFromDisk(right + r, buf);
		if (*blk1<*blk2)
		{
			writeBlockToDisk((unsigned char *)blk1, tmpaddr + offset, buf);
			l++;
			offset++;
		}
		else
		{
			writeBlockToDisk((unsigned char *)blk2, tmpaddr + offset, buf);
			r++;
			offset++;
		}
		freeBlockInBuffer((unsigned char *)blk1, buf);
		freeBlockInBuffer((unsigned char *)blk2, buf);
	}
	for (i = 0; i<offset; i++)//WB
	{
		blk1 = (unsigned int*)readBlockFromDisk(tmpaddr + i, buf);
		writeBlockToDisk((unsigned char *)blk1, left + i, buf);
		freeBlockInBuffer((unsigned char *)blk1, buf);
	}
	return left;
}

void bin_search(){
    S rs[224];
    S res[7];
     for(int i=0;i<7;i++)
    {
        res[i].c = 0;
        res[i].d = 0;
    }
    int cnt = 0;
    for(int j=16;j<=47;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            int c = *(blk + 8*i );
            int d = *(blk + 8*i + 4);
            rs[cnt].c = c;
            rs[cnt].d = d;
            cnt++;
        }
    }
    freeBlockInBuffer(blk,&db);

    for (int i = 0; i <= cnt; i++) {
			for (int j = i; j > 0 && rs[j-1].c > rs[j].c; j--) {
				S temp = rs[j];
				rs[j] = rs[j-1];
				rs[j-1] = temp;
			}
    }

	for(int i=0;i<cnt;i++)
    {
        blk = getNewBlockInBuffer(&db);
        *(blk) = rs[i].c;
        *(blk + 4) = rs[i].d;
        writeBlockToDisk(blk,baddr[i],&db);
        freeBlockInBuffer(blk,&db);
    }

    int small = 0;
    int big = 223;
    int mid;
    while(small<big)
    {
        mid = (small+big)/2;
        blk = readBlockFromDisk(baddr[mid],&db);
        if(*(blk)>60) big = mid + 1;
        else if(*(blk)<60) small = mid-1;
        else{
            freeBlockInBuffer(blk,&db);
            break;
        }
        freeBlockInBuffer(blk,&db);
    }
    readBlockFromDisk(baddr[mid],&db);
    int u = *blk;
    int v = *(blk+4);
    cout << u << " " << v << endl;

    //在mid 值两边查询
    int m = mid-1;
    while(m>0)
    {
        blk = readBlockFromDisk(baddr[m],&db);
        if(*blk == 60 ) {
            u = *blk;
            v = *(blk+4);
            cout << u << " " << v << endl;
            freeBlockInBuffer(blk,&db);
            m--;
        }
        else{
            freeBlockInBuffer(blk,&db);
            break;
        }
    }

    while(m<224)
    {
        blk = readBlockFromDisk(baddr[m],&db);
        if(*blk == 60 ) {
            u = *blk;
            v = *(blk+4);
            cout << u << " " << v << endl;
            freeBlockInBuffer(blk,&db);
            m++;
        }
        else{
            freeBlockInBuffer(blk,&db);
            break;
        }
    }

    ////////////////////////////////////////////////////
    int low = 0,high = cnt,temp,cont = 0;
    while(low <= high){
        int mid = rs[(low+high)/2].c;
        if(mid > 60) high = (low+high)/2 -1;
        else if(mid < 60) low = (low+high)/2 +1;
        else {
            temp = (low+high)/2;
            break;
        }
    }
    int k = temp;
    while(temp <= cnt){
        if(rs[temp].c != 60) break;
        res[cont] = rs[temp];
        cont++;
        temp++;
    }
    temp = k;
    while(temp >= 0){
        if(rs[temp].c != 60) break;
        res[cont] = rs[temp];
        cont++;
        temp--;
    }
    freeBlockInBuffer(blk,&db);
    blk = getNewBlockInBuffer(&db);
    for(int i = 0;i < 7;i++){
        *(blk+8*i) = res[i].c;
        *(blk+8*i+4) =res[i].d;
    }
    if (writeBlockToDisk(blk, blkAddr[51],&db) != 0) return ;
    freeBlockInBuffer(blk,&db);
    cout << "二分搜索的结果是：" << endl;
    for(int i = 1;i < cont;i++){
        cout << res[i].c << " " << res[i].d << endl;
    }
    cout <<"结果写入第51个磁盘块。"<<endl;
    return;

}
void projectN()
{
    R rs[112];
    int cnt = 0;
    for(int j=0;j<16;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            rs[cnt].a = *(blk + 8*i );
            rs[cnt].b = *(blk + 8*i + 4);
            cnt++;
            //cout <<"a" << a << endl;
        }
    }
    for(int i=0;i<cnt-1;i++)
    {
        for(int j=i+1;j<cnt;j++)
        {
            if(rs[i].a < rs[j].a)
            {
                R temp = rs[i];
                rs[i] = rs[j];
                rs[j] = temp;
            }
        }
    }
    int a = rs[0].a;
    for(int i=1;i<cnt;i++)
    {
        if(rs[i].a==a) continue;
        else{
            a=rs[i].a;
            cout << a << "\t" ;
        }
    }
}
set<int> project()//R.A=40
{
    set<int> pro;

    for(int j=0;j<16;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            int a = *(blk + 8*i );
            pro.insert(a);
            //cout <<"a" << a << endl;
        }
    }
    freeBlockInBuffer(blk,&db);

    blk = getNewBlockInBuffer(&db);
    //一个新块
    int i=0;
    set<int>::reverse_iterator rit;
    cout << "R.A Project:" << endl;
    rit=pro.rbegin();
    rit++;

    int g  = 0;
    int ti = 0;
    blk = getNewBlockInBuffer(&db);
    cout << "投影的结果是：" << endl;
    for(;rit!=pro.rend();rit++)
    {
        cout << *rit << "\t";
        *( blk+4*g ) = *rit;
        g++;
        if(g == 15){
            writeBlockToDisk(blk,blkAddr[52+ti],&db);
            ti++ ;
            freeBlockInBuffer(blk,&db);
            g=0;
            blk = getNewBlockInBuffer(&db);
        }
    }
    if(g>0)
    {
        writeBlockToDisk(blk,blkAddr[52+ti],&db);
        freeBlockInBuffer(blk,&db);
    }

    if(blk!=NULL) freeBlockInBuffer(blk,&db);
    cout << endl << "磁盘块52-" << 52+ti << endl;
    return pro;
}

void nextloopjoin(){
    int ri, rj, si, sj, cont = 0;
    RS nlj[1120];
	int a, b, c, d;
	unsigned char *desk_blk = NULL;
	for (ri = 0; ri<16; ri++)
	{
		if(blk!=NULL) freeBlockInBuffer(blk,&db);
		blk = readBlockFromDisk(ri+1, &db);
		for (rj = 0; rj<7; rj++)
		{
			a = *(blk + 8 * rj);
			for (si = 16; si<48; si++)
			{
				if(desk_blk!=NULL) freeBlockInBuffer(desk_blk,&db);
				desk_blk = readBlockFromDisk(si+1, &db);
				for (sj = 0; sj<7; sj++)
				{
					c = *(desk_blk + 8 * sj);
					if (a == c && c <= 40)
					{
						b = *(blk + 8 * rj + 4);
						d = *(desk_blk + 8 * sj + 4);
						nlj[cont].a = a;
						nlj[cont].b = b;
						nlj[cont].d = d;
						cont++;
					}
				}
			}
		}
	}

	int g  = 0;
    int ti = 0;
    blk = getNewBlockInBuffer(&db);
    for(int i=0;i<cont;i++)
    {
        *( blk+12*g+0 ) = nlj[i].a;
        *( blk+12*g+4 ) = nlj[i].b;
        *( blk+12*g+8 ) = nlj[i].d;
        //i++;
        g++;
        if(g == 5){
            writeBlockToDisk(blk,blkAddr[100+ti],&db);
            ti++ ;
            freeBlockInBuffer(blk,&db);
            blk = getNewBlockInBuffer(&db);
            g=0;
        }
    }
    if(g>0)
    {
        writeBlockToDisk(blk,blkAddr[100+ti],&db);
        freeBlockInBuffer(blk,&db);
    }
    if(g==0)freeBlockInBuffer(blk,&db);
    cout << "NEXT-LOOP-JOIN所存储的磁盘块 100-" << 100+ti << endl;
    cout << "Join的结果有" << cont << "个。" << endl;
}

void sortMergeJoin()
{
    int sum = 0;
    R rs[112];
    S ss[224];

    RS result[500];
    set<int> sset;

    int cnt = 0;
    for(int i=0;i<16;i++)
    {
        blk = readBlockFromDisk(blkAddr[i],&db);
        for(int j=0;j<7;j++)
        {
            rs[cnt].a = *(blk + j*8);
            rs[cnt].b = *(blk + j*8 + 4);
            cnt++;
        }
        freeBlockInBuffer(blk,&db);
    }
    cnt = 0;
    for(int i=16;i<48;i++)
    {
        blk = readBlockFromDisk(blkAddr[i],&db);
        for(int j=0;j<7;j++)
        {
            ss[cnt].c = *(blk + j*8);
            ss[cnt].d = *(blk + j*8 + 4);
            cnt++;
        }
        freeBlockInBuffer(blk,&db);
    }

    //由于只考虑a = c仍然只需要把p指向>20处
    for(int i=0;i<111;i++)
    {
        for(int j=i+1;j<112;j++)
        {
            if(rs[i].a>rs[j].a)
            {
                R temp;
                temp.a = rs[i].a;  temp.b = rs[i].b;
                rs[i].a = rs[j].a; rs[i].b = rs[j].b;
                rs[j].a = temp.a;  rs[j].b = temp.b;
            }
        }
    }
    for(int i=0;i<223;i++)
    {
        for(int j=i+1;j<224;j++)
        {
            if(ss[i].c>ss[j].c)
            {
                S temp;
                temp.c = ss[i].c;  temp.d = ss[i].d;
                ss[i].c = ss[j].c; ss[i].d = ss[j].d;
                ss[j].c = temp.c;  ss[j].d = temp.d;
            }
        }
    }
    int p = 0;
    int q = 0;
    while(rs[p].a<20) p++;

    while(ss[q].c<=40)
    {
        sset.clear();
        int f = ss[q].c;
        sset.insert(ss[q].d);
        q++;
        while(ss[q].c==f){
            q++;
            sset.insert(ss[q].d);
        }
        while(rs[p].a<f) p++;
        while(rs[p].a ==f)
        {
            set<int>::reverse_iterator rit;
            for(rit=sset.rbegin();rit!=sset.rend();rit++)
            {
                result[sum].a = f;
                result[sum].b = rs[p].b;
                result[sum].d = *rit;
                sum++;
            }
            p++;
        }
    }

    int g  = 0;
    int ti = 0;
    blk = getNewBlockInBuffer(&db);
    for(int i=0;i<sum;i++)
    {
        *( blk+12*g+0 ) = result[i].a;
        *( blk+12*g+4 ) = result[i].b;
        *( blk+12*g+8 ) = result[i].d;
        //i++;
        g++;
        if(g == 5){
            writeBlockToDisk(blk,blkAddr[200+ti],&db);
            ti++ ;
            freeBlockInBuffer(blk,&db);
            g=0;
            blk = getNewBlockInBuffer(&db);
        }
    }
    if(g>0)
    {
        writeBlockToDisk(blk,blkAddr[200+ti],&db);
        freeBlockInBuffer(blk,&db);
    }
    else
    {
        freeBlockInBuffer(blk,&db);
    }
    cout << "SORT-MERGE-JOIN所存储的磁盘块 200-" << 200+ti << endl;
    cout << "结果有" << sum << "个。" << endl;

}
void hashjoin(){
    hajo hj[19];
    int countnum[19];
    memset(countnum,0,sizeof(int)*19);
    R rs[112];
    RS res[700];
    int cont = 0;
    for(int j = 0;j < 16;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            int a = *(blk + 8*i );
            int b = *(blk + 8*i + 4);
            hj[a%19].anum[countnum[a%19]] = a;
            hj[a%19].bnum[countnum[a%19]] = b;
            countnum[a%19]++;
        }
    }
    freeBlockInBuffer(blk,&db);
    for(int j = 16;j < 48;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++){
            int c = *(blk + 8*i );
            int d = *(blk + 8*i +4);
            for(int k = 0;k < countnum[c%19];k++){
                if(c == hj[c%19].anum[k] && c <= 40){
                    res[cont].a = c;
                    res[cont].b = hj[c%19].bnum[k];
                    res[cont].d = d;
                    cont++;
                }
            }
        }
    }
    freeBlockInBuffer(blk,&db);

    int g  = 0;
    int ti = 0;
    blk = getNewBlockInBuffer(&db);
    for(int i=0;i<cont;i++){
        *( blk+12*g+0 ) = res[i].a;
        *( blk+12*g+4 ) = res[i].b;
        *( blk+12*g+8 ) = res[i].d;
        //i++;
        g++;
        if(g == 5){
            writeBlockToDisk(blk,blkAddr[300+ti],&db);
            ti++ ;
            freeBlockInBuffer(blk,&db);
            g=0;
            blk = getNewBlockInBuffer(&db);
        }
    }
    if(g>0){
        writeBlockToDisk(blk,blkAddr[300+ti],&db);}
    freeBlockInBuffer(blk,&db);

    cout << "HASH-JOIN所存储的磁盘块 300-" << 300+ti << endl;
    cout << "结果有" << cont << "个。" << endl;
}
void unionrelationship(){//R:a in (20,30) or b  in (10,200)
    hajo ur[10];
    int countnum[10];
    R res[112];
    int cont = 0;
    bool flag = true;
    for(int j = 0;j < 16;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            int a = *(blk + 8*i );
            int b = *(blk + 8*i + 4);
            if(a > 20 && a < 30){
                for(int k = 0;k < countnum[a%10];k++){
                    if(a == ur[a%10].anum[k]){
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    ur[a%10].anum[countnum[a%10]] = a;
                    ur[a%10].bnum[countnum[a%10]] = b;
                    countnum[a%10]++;
                }
            }
        }
    }
    freeBlockInBuffer(blk,&db);
    for(int j = 0;j < 16;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            int a2 = *(blk + 8*i );
            int b2 = *(blk + 8*i +4);
            for(int k = 0;k < countnum[a2%10];k++){
                if(a2 != ur[a2%10].anum[k] && b2 < 200 && b2 > 10 && b2 != ur[a2%10].bnum[k]){

                    res[cont].a = ur[a2%10].anum[k];
                    res[cont].b = ur[a2%10].bnum[k];
                    cont++;
                    res[cont].a = a2;
                    res[cont].b = b2;
                    cont++;
                }
            }
        }
    }
    freeBlockInBuffer(blk,&db);

    int g  = 0;
    int ti = 0;
    blk = getNewBlockInBuffer(&db);
    for(int i=0;i<cont;i++){
        *( blk+8*g+0 ) = res[i].a;
        *( blk+8*g+4 ) = res[i].b;
        //i++;
        g++;
        if(g==7){
            writeBlockToDisk(blk,blkAddr[60+ti],&db);
            ti++ ;
            freeBlockInBuffer(blk,&db);
            g=0;
            blk = getNewBlockInBuffer(&db);}
    }
    if(g>0){
        writeBlockToDisk(blk,blkAddr[60+ti],&db);
    }
    freeBlockInBuffer(blk,&db);
    cout << "Union所存储的磁盘块 60-" << 60+ti << endl;
    cout << "结果有" << cont << "个。" << endl;
}
void intersectionrelationship(){//R:a in (20,30) or b  in (10,200)
    hajo ur[10];
    int countnum[10];
    R res[112];
    int cont = 0;
    bool flag = true;
    for(int j = 0;j < 16;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            int a = *(blk + 8*i );
            int b = *(blk + 8*i + 4);
            if(a > 20 && a < 30){
                for(int k = 0;k < countnum[a%10];k++){
                    if(a == ur[a%10].anum[k]){
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    ur[a%10].anum[countnum[a%10]] = a;
                    ur[a%10].bnum[countnum[a%10]] = b;
                    countnum[a%10]++;
                }
            }
        }
    }
    freeBlockInBuffer(blk,&db);
    for(int j = 0;j < 16;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            int a2 = *(blk + 8*i );
            int b2 = *(blk + 8*i +4);
            for(int k = 0;k < countnum[a2%10];k++){
                if(a2 == ur[a2%10].anum[k] && b2 < 200 && b2 > 10 && b2 == ur[a2%10].bnum[k]){
                    res[cont].a = a2;
                    res[cont].b = b2;
                    cont++;
                }
            }
        }
    }
    freeBlockInBuffer(blk,&db);

    int g  = 0;
    int ti = 0;
    blk = getNewBlockInBuffer(&db);
    for(int i=0;i<cont;i++){
        *( blk+8*g+0 ) = res[i].a;
        *( blk+8*g+4 ) = res[i].b;
        g++;
        if(g==7){
            writeBlockToDisk(blk,blkAddr[70+ti],&db);
            ti++ ;
            freeBlockInBuffer(blk,&db);
            g=0;
            blk = getNewBlockInBuffer(&db);}
    }
    if(g>0){
        writeBlockToDisk(blk,blkAddr[70+ti],&db);
    }
    freeBlockInBuffer(blk,&db);
    cout << "Intersection所存储的磁盘块 70-" << 70+ti << endl;
    cout << "结果有" << cont << "个。" << endl;
}
void differencerelationship(){//R：b in (10,200) - a in (20,30)
    hajo ur[10];
    int countnum[10];
    R res[112];
    int cont = 0;
    bool flag = true;
    for(int j = 0;j < 16;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            int a = *(blk + 8*i );
            int b = *(blk + 8*i + 4);
            if(a > 20 && a < 30){
                for(int k = 0;k < countnum[a%10];k++){
                    if(a == ur[a%10].anum[k]){
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    ur[a%10].anum[countnum[a%10]] = a;
                    ur[a%10].bnum[countnum[a%10]] = b;
                    countnum[a%10]++;
                }
            }

        }
    }
    freeBlockInBuffer(blk,&db);
    for(int j = 0;j < 16;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            int a2 = *(blk + 8*i );
            int b2 = *(blk + 8*i +4);
            for(int k = 0;k < countnum[a2%10];k++){
                if(a2 != ur[a2%10].anum[k] && b2 < 200 && b2 > 10 && b2 != ur[a2%10].bnum[k]){

                    res[cont].a = a2;
                    res[cont].b = b2;
                    cont++;
                }
            }
        }
    }
    freeBlockInBuffer(blk,&db);

    int g  = 0;
    int ti = 0;
    blk = getNewBlockInBuffer(&db);
    for(int i=0;i<cont;i++){
        *( blk+8*g+0 ) = res[i].a;
        *( blk+8*g+4 ) = res[i].b;
        //i++;
        g++;
        if(g==7){
            writeBlockToDisk(blk,blkAddr[80+ti],&db);
            ti++ ;
            freeBlockInBuffer(blk,&db);
            g=0;
            blk = getNewBlockInBuffer(&db);}
    }
    if(g>0){
        writeBlockToDisk(blk,blkAddr[80+ti],&db);
    }
    freeBlockInBuffer(blk,&db);
    cout << "Difference所存储的磁盘块 80-" << 80+ti << endl;
    cout << "结果有" << cont << "个。" << endl;
}
void hash_search(){//R.a = 40;
    hajo hj[19];
    int countnum[19];
    memset(countnum,0,sizeof(int)*19);
    R rs[112];
    int cont = 0;
    for(int j = 0;j < 16;j++)
    {
        if(blk!=NULL) freeBlockInBuffer(blk,&db);
        blk = readBlockFromDisk(blkAddr[j], &db);
        for (int i = 0; i < 7; i++)
        {
            int a = *(blk + 8*i );
            int b = *(blk + 8*i + 4);
            hj[a%19].anum[countnum[a%19]] = a;
            hj[a%19].bnum[countnum[a%19]] = b;
            countnum[a%19]++;

        }
    }
    freeBlockInBuffer(blk,&db);
    for(int k = 0;k < countnum[40%19];k++){
        if(hj[40%19].anum[k] == 40){
            rs[cont].a = 40;
            rs[cont].b = hj[40%19].bnum[k];
            cont++;
         }
    }

    int g  = 0;
    int ti = 0;
    blk = getNewBlockInBuffer(&db);
    for(int i=0;i<cont;i++){
        *( blk+8*g+0 ) = rs[i].a;
        *( blk+8*g+4 ) = rs[i].b;
        //i++;
        cout << rs[i].a << " " << rs[i].b << endl;
        g++;
        if(g==7){
            writeBlockToDisk(blk,blkAddr[90+ti],&db);
            ti++ ;
            freeBlockInBuffer(blk,&db);
            g=0;
            blk = getNewBlockInBuffer(&db);}
    }
    if(g>0){
        writeBlockToDisk(blk,blkAddr[90+ti],&db);
    }
    freeBlockInBuffer(blk,&db);
    cout << "Search所存储的磁盘块 90-" << 90+ti << endl;
    cout << "结果有" << cont << "个。" << endl;

}


int main(int argc, char **argv)
{

    printf("HelloWorld!");
    cout << endl;

    for(int i =0;i<224;i++)
    {
        baddr[i] = i + 450;
    }

    for(int i=0;i<allBlk;i++)
    {
        blkAddr[i] = i+1;
    }

    initBuffer( bufSize, blkSize, &db ); //初始化缓冲区

    int j = 0;
    //编造数据
    //0-15 R 16-47 S
    while(j<48)
    {
        blk = getNewBlockInBuffer(&db);
        for (int i = 0; i < 7; i++)
        {
            if(j<16){
                *(blk + 8*i ) = rand()%40 + 1;
            }
            else{
                *(blk + 8*i ) = rand()%40 + 21;
            }
            *(blk + 8*i + 4 ) = rand()%250 + 1;

            //int x = *(blk+8*i);
            //int y = *(blk+8*i+4);
            //if(j==31)cout << x << " and " << y << ".";
            //if(j==47)cout << x << " and " << y << ".";
        }
        if (writeBlockToDisk(blk, blkAddr[j],&db) != 0) return 0;
        freeBlockInBuffer(blk,&db);
        j++;
        //cout << endl << endl;
    }
    //initBuffer(bufSize, blkSize, &db);blk = readBlockFromDisk(blkAddr[47],&db);
    /*blk = readBlockFromDisk(blkAddr[0],&db);
    for(int x = 0;x< 7;x++)
    {
        int p = *(blk+8*x);
        int q = *(blk+8*x+4);
        cout << p << " and " << q << ".";
    }
    freeBlockInBuffer(blk,&db);*/
    /*cout <<endl << endl;
    blk = readBlockFromDisk(blkAddr[47],&db);
    for(int x = 0;x< 7;x++)
    {
        int p = *(blk+8*x);
        int q = *(blk+8*x+4);
     x   cout << p << " and " << q << ".";
    }*/
    cout << endl;
    unionrelationship();
    cout << endl;
    intersectionrelationship();
    cout << endl;
    differencerelationship();

    cout << endl;
    linear_search();
    cout << endl;
    bin_search();
    cout << endl;
    hash_search();

    cout << endl;
    project();
    cout << endl;
    projectN();

    cout << endl;
    nextloopjoin();
    cout << endl;
    sortMergeJoin();
    cout << endl;
    hashjoin();
    cout << endl;

    cout << db.numIO;

    return 0;
}


