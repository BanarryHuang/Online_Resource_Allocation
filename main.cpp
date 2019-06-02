#include <fstream>
#include <random>
#include <vector>
#define N 400
using namespace std;
class Request
{
    int id;
    bool matched;
    int resourceId;
    double weight;
public:
    Request(int newID, double newweight);
    friend class Resource;
    int read_id();
    int read_resid();
    double read_weight();
    bool read_matched();
    static int sat_Request;
    static void add_sat();
    static int get_sat();
};
int Request::sat_Request=0;
Request::Request(int newID, double newweight)
{
    id=newID;
    matched=false;
    resourceId=-1;
    weight=newweight;
}
void Request::add_sat()
{
    sat_Request++;
}
int Request::get_sat()
{
    return sat_Request;
}
int Request::read_id(){
    return id;
}
int Request::read_resid(){
    return resourceId;
}
double Request::read_weight()
{
    return weight;
}
bool Request::read_matched()
{
    return matched;
}
class Resource
{
    int id;
    bool matched;
    int requestId;
public:
    Resource(int newID);
    int read_id();
    bool operator->* (Request &b){
        if(matched==false&&b.matched==false){
            requestId=b.id;
            b.resourceId=id;
            matched=true;
            b.matched=true;
            return true;
        }
        else
            return false;
    }
};
Resource::Resource(int newID)
{
    id=newID;
    matched=false;
    requestId=-1;
}
int Resource::read_id()
{
    return id;
}
int main()
{
    ifstream inStream;
    ofstream outStream;
    inStream.open("request.txt");
    outStream.open("result.txt");
    int i,j,timeslot,timeslotD,Resource_add,Request_add,newID;
    int edge,edgeD,resID,reqID,temp,max_weight,sel_req;
    vector <int> row;
    row.assign(N,0);
    vector < vector <int> > array_2D;
    array_2D.assign(N,row);
    default_random_engine gen = default_random_engine(1);
    uniform_real_distribution<double> dis(0,1);
    vector <Resource> res;
    vector <Request> req;
    inStream >> timeslot;
    while(timeslot--){
        inStream >> timeslotD >> Request_add >> Resource_add;
        while(Request_add--){
            inStream >> newID;
            req.push_back(Request(newID,dis(gen)));
        }
        while(Resource_add--){
            inStream >> newID;
            res.push_back(Resource(newID));
        }
        inStream >> edge;
        temp=edge;
        while(temp--){
            inStream >> edgeD >> reqID >> resID;
            array_2D[resID].push_back(reqID);
        }
        for(i=0;i<res.size();i++){
            if(array_2D[res[i].read_id()].empty()==false){
            	max_weight=0;
                for(j=0;j<array_2D[res[i].read_id()].size();j++){
                    if(req[array_2D[res[i].read_id()][j]].read_weight()>max_weight&&req[array_2D[res[i].read_id()][j]].read_matched()==false){
                        max_weight=req[array_2D[res[i].read_id()][j]].read_weight();
                        sel_req=array_2D[res[i].read_id()][j];
                    }
                }
                if(res[i]->*req[sel_req]==true)
                    req[0].add_sat();
                array_2D[res[i].read_id()].clear();
            }
        }
    }
    outStream << req[0].get_sat() << endl;
    for(i=0;i<req.size();i++)
        if(req[i].read_resid()!=-1)
            outStream << req[i].read_id() << ' ' << req[i].read_resid() << endl;
    inStream.close();
    outStream.close();
    return 0;
}
