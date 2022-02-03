// We have a two-dimensional array of Integer called nums, 
// it saves the length and width of cards, we try to stack these cards, 
// the big card must under the small one, 
// it means the length and width of the smaller card 
// must be less than or equal to the length and width of the larger card, 
// Return a list of the highest stack, keep the order by input list
// Input: [[5,2],[1,3],[2,5],[10,1],[3,4],[2,7],[1,1],[3,1]]
// Output: [[2,7],[5,2],[2,5],[1,3],[3,1],[1,1]]     
// Explain: you can rotate [5,2], then it is [2,5], so [5,2] can put on [2,7], 
// by the same way, [2,5] can put on [5,2], and in the input list, [2,5] after [5,2], 
// so [2,5] should after [5,2] in the output.

// [10, 1] [1, 3] [3, 1] [1, 1] is not a solution,    [10,1] [2,7] [5,2] [2,5] [3,4] ,[3,1] ,[1,1]
// because the height of the cards is 4 which is lower than 6


#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

class CARD;
bool comparator(pair<int,int>,pair<int,int>);
bool isCoverable(pair<int,int>&,pair<int,int>&);
void printVec(vector<CARD*>&);

class CARD{

  public:
    pair<int,int> p;
    int num_coverable;
    vector<CARD*> coverable_set;

  public:
    CARD():p(pair<int,int>(0,0)),num_coverable(0),coverable_set(){}
    CARD(pair<int,int>& p1):p(p1),num_coverable(0),coverable_set(){}

  public:
    void getCoverableSet(unique_ptr<CARD[]>& u_ptr,size_t& i,size_t& num_size){
        for(auto j =i+1; j < num_size ; j++){
            if(isCoverable(p,u_ptr[j].p)){
                coverable_set.emplace_back(&(u_ptr[j]));   // 这里必须将对象的地址压入set，否则压入对象只是复制对象，该压入对象的实体发生改变不会影响此复制体。
            }
        }
    }
};

void printVec(vector<CARD*>& card_nums){
  for (size_t i = 0; i < card_nums.size(); i++){
      cout <<"[" << (*(card_nums[i])).p.first <<"," << (*(card_nums[i])).p.second <<"]"<< endl;
     }
}

void printVec2(vector<CARD>::iterator it_b, vector<CARD>::iterator it_e){
  for (auto i = it_b; i !=it_e; i--){
      cout <<"[" << (*i).p.first <<"," << (*i).p.second <<"]"<< endl;
     }
}

void printVec2(vector<CARD>::reverse_iterator it_rb, vector<CARD>::reverse_iterator it_re){
  for (auto i = it_rb; i !=it_re; i++){
      cout <<"[" << (*i).p.first <<"," << (*i).p.second <<"]"<< endl;
     }
}

bool comparator(pair<int,int> p1,pair<int,int> p2){   // 这个函数这里不能使用引用
        
        int p1_max = (p1.first>=p1.second)?p1.first:p1.second;
        int p2_max = (p2.first>=p2.second)?p2.first:p2.second;

        return(p1_max > p2_max);    
}

bool isCoverable(pair<int,int>& p1,pair<int,int>& p2){
      int p1_max = max(p1.first,p1.second);
      int p1_min = min(p1.first,p1.second);
      int p2_max = max(p2.first,p2.second);
      int p2_min = min(p2.first,p2.second);
      return (p1_max >= p2_max) && (p1_min >= p2_min);
}

vector<CARD>& getDepth(CARD& this_card, vector<CARD>& path_res){
    int path_len = 1;
    int max_len = 0;
    //path_res.push_back(this_card);
    //cout << "[" << this_card.p.first << "," << this_card.p.second <<"]"<<endl;
    //cout << "size:" << this_card.coverable_set.size() <<endl;
    if(!this_card.coverable_set.empty()){
      for(size_t i = 0; i < this_card.coverable_set.size();i++){
         int temp_len = 0;
         vector<CARD> path_temp;
         path_temp.clear();
         path_temp = getDepth(*(this_card.coverable_set[i]),path_temp); //这里第一项我们要解引用。
         
         if (path_temp.size() < max_len)
            path_temp.clear();
         else{
            max_len = path_temp.size();
            path_res = path_temp;
         }
         //cout << "max_len:" << max_len << endl;
      }
      path_res.push_back(this_card);
    }
    else{
        path_res.push_back(this_card);
    }
    return path_res;
}

int main(){
  
  vector<pair<int,int>> nums={{5,2},{1,3},{2,5},{10,1},{3,4},{2,7},{1,1},{3,1}};
  
  sort(nums.begin(),nums.end(),comparator);

  auto itr_b = nums.begin();
  auto itr_e = nums.end();
  auto num_size = nums.size();
  
  unique_ptr<CARD[]> u_ptr(new CARD[nums.size()]);

  for(size_t i = 0; i < nums.size();i++){
      u_ptr[i].p = nums[i];
      //cout << "[" << u_ptr[i].p.first << "," << u_ptr[i].p.second <<"]"<<endl;
  }

  for(size_t i = 0; i < nums.size();i++){
      u_ptr[i].getCoverableSet(u_ptr,i,num_size);
  }

  vector<CARD> path_res;
  vector<CARD> path_temp;

  int max_len = 0;
  cout << "Here" <<endl;
  //printVec((*(u_ptr[0].coverable_set[0])).coverable_set);
  //cout<<u_ptr[5].coverable_set.empty()<<endl;
  for(size_t i = 0; i<nums.size();i++){
        path_temp = getDepth(u_ptr[i],path_temp);   //这里返回的引用
        if(path_temp.size()>max_len){
            max_len = path_temp.size();
            path_res.assign(path_temp.begin(),path_temp.end());
        }
        //path_temp.clear();
  }
  printVec2(path_res.rbegin(),path_res.rend());
  //cout << u_ptr[1].p.first <<","<<u_ptr[1].p.second <<endl;
  //cout << u_ptr[2].p.first <<","<<u_ptr[2].p.second <<endl;
  //cout << isCoverable(u_ptr[0].p,u_ptr[1].p) <<endl;
  return 0;
}
