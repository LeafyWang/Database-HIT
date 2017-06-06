# -*- coding: utf-8 -*-
class tree:
    def __init__(self):
        self.thing = ''
        self.condition=''
        self.lchild=-1
        self.rchild=-1

def draw(new):
    for i in new:
        if i.thing=='sel' or i.thing=='Pro':
            print '             ',i.thing,i.condition
            print '                   '
            print '                       |           '
        if i.thing=='Join':
            print '                     ',i.thing
            print '                 /             \ '
            print '             ',new[i.lchild].thing,'         ',new[i.rchild].thing

def opt(testquery):
    query_tree=tree()
    new = []
    for i in range(0,len(testquery)):
        if testquery[i]=='SELECT':
            query_tree=tree()
            query_tree.thing='sel'
            j=i
            while 1:
                if testquery[j]!=']':
                    j=j+1
                else:
                    break
            for q in range(i+2,j):
                query_tree.condition+=testquery[q]
            i=j+1
            new.append(query_tree)
            if new.index(query_tree)!=0:
                new[-2].lchild=new.index(query_tree)
            continue
        elif testquery[i]=='PROJECTION':
            query_tree=tree()
            query_tree.thing='Pro'
            j=i
            while 1:
                if testquery[j]!=']':
                    j=j+1
                else:
                    break
            for q in range(i+2,j):
                query_tree.condition+=testquery[q]
            i=j+1
            new.append(query_tree)
            if new.index(query_tree)!=0:
                new[-2].lchild=new.index(query_tree)
            continue
        elif testquery[i]=='JOIN':
            query_tree=tree()
            query_tree.thing='Join'
            query_ltree=tree()
            query_ltree.thing=testquery[i-1].strip('(')
            query_rtree=tree()
            query_rtree.thing=testquery[i+1].strip(')')
            new.append(query_tree)
            new[-2].lchild=new.index(query_tree)
            query_tree.lchild=new.index(query_tree)+1
            query_tree.rchild=new.index(query_tree)+2
            new.append(query_ltree)
            new.append(query_rtree)

    draw(new);

    temp_S = []
    temp_P = []
    temp_key = []
    temp_chart = []
    better = []
    for item in new:
        if item.thing == 'sel':
            T = []
            T=(item.condition.split('&'))
            for i in T:
                temp_S.append(i)
        if item.thing == 'Pro':
            T = []
            T=(item.condition.split(','))
            for i in T:
                temp_P.append(i)
        elif item.thing =='Join':
            T=[]
            T.append(new[item.lchild].thing)
            T.append(new[item.rchild].thing)
            temp_chart.append(T)
    for item in temp_chart:
            better_tree=tree()
            better_tree.thing='Join'
            better_ltree=tree()
            better_ltree.thing=item[0]
            better_rtree=tree()
            better_rtree.thing=item[1]
            better.append(better_tree)
            better[-1].lchild=better.index(better_tree)
            better_tree.lchild=better.index(better_tree)+1
            better_tree.rchild=better.index(better_tree)+2
            better.append(better_ltree)
            better.append(better_rtree)
            if set(dic[item[0]])&set(dic[item[1]])!=():
                for i in (list(set(dic[item[0]])&set(dic[item[1]]))):
                    temp_key.append(i)
    hasbeen = []
    Proj = []
    for i in range(0,9999):
        if i >=len(better):
            break
        if better[i].thing=='Join':
            continue
        elif better[i].thing in dic.keys():
            if i not in hasbeen:
                a = i
                for j in temp_S:
                    if better[a].thing in dic.keys():
                        if j[:j.index('=')] in dic[better[a].thing]:
                            better_tree=tree()
                            better_temp=tree()
                            better_tree.thing = 'sel'
                            better_tree.condition = j
                            better_temp = better[i]
                            better[i] = better_tree
                            better.append(better_temp)
                            better_tree.lchild=better.index(better_temp)
                            hasbeen.append(better_tree.lchild)
                            a = better_tree.lchild
                if temp_P!=[]:
                    for k in temp_P+temp_key:
                        if better[a].thing in dic.keys():
                            if k in dic[better[a].thing] and k not in Proj:
                                Proj.append(k)
                    better_tree=tree()
                    better_temp=tree()
                    better_tree.thing = 'Pro'
                    better_tree.condition = str(Proj)
                    better_temp = better[i]
                    better[i] = better_tree
                    better.append(better_temp)
                    better_tree.lchild=better.index(better_temp)
                    hasbeen.append(better_tree.lchild)
                    a = better_tree.lchild
    if temp_P!=[]:
         better_tree = tree()
         better_tree.thing='Pro'
         better_tree.condition = temp_P
         better_tree.lchild = 1
         for i in better:
             if i.lchild!=-1:
                 i.lchild= i.lchild+1
             if i.rchild!=-1:
                 i.rchild=i.rchild+1
         better.insert(0,better_tree)

    flag = 0
    for i in better:
        if flag:
            print i.thing, i.condition + "  :L----R:  ",
            flag = 0
        else:
            print i.thing, i.condition
            print "\t|\n\tV"
        if i.lchild != -1 and i.rchild != -1:
            flag = 1

    print "\n======================================================\n"

sql1 = "SELECT [ ENAME = 'Mary' & DNAME = 'Research' ] ( EMPLOYEE JOIN DEPARTMENT )"
sql2 = "PROJECTION [ BDATE ] ( SELECT [ ENAME = 'John' & DNAME = 'Research' ] ( EMPLOYEE JOIN DEPARTMENT ) )"
sql3 = "SELECT [ ESSN = '01' ] ( PROJECTION [ ESSN , PNAME ] ( WORKS_ON JOIN PROJECT ) )"
sql4 = "PROJECTION [ DNAME, SALARY ] ( AVG [ SALARY ] ( SELECT [ DNAME = 'Research' ] ( EMPLOYEE JOIN DEPARTMENT ) )"
sql5 = "PROJECTION [ ENAME ] ( SELECT [ SALARY < 3000 ] ( EMPLOYEE JOIN SELECT [ PNO = 'P1' ] ( WORKS_ON JOIN PROJECT ) )"

dic = dict()
fp = open("table.txt",'r')
for line in fp.readlines():
    line = line.strip()
    line = line.split(',')
    dic[line[0]]=[]
    for i in range(1,len(line)):
        dic[line[0]].append(line[i])
fp.close()

opt(sql1.split(' '))
opt(sql2.split(' '))
opt(sql3.split(' '))

print "This is an adaption.\n"
print "origin edition by Yang Han, thanks a lot.\n"
m = raw_input()

