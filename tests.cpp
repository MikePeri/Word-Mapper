/**
 * Demonstrates STL set algorithms.
 * 
 * @author Erel Segal-Halevi
 * @since 2019-05
 */

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <array>
#include <set>
#include <regex>
#include <cmath> 
using namespace std;



int main() {
	set<int> v1{2,5,9,13,19,89,91};
	set<int> v2{3,10,15,20,90};
	auto ret_lines = make_shared<set<int>>();
    set<int>::iterator it;
    for (auto i = v1.begin(); i != v1.end(); ++i)
    {
        int value = (*i);
        it = find(v2.begin(),v2.end(),value+1);
        if(it != v2.end())
            {
                ret_lines->insert(value);
                ret_lines->insert(value+1);
            }//if
        it = find(v2.begin(),v2.end(),value-1);
        if(it != v2.end())
            {
                ret_lines->insert(value-1);
                ret_lines->insert(value);
            }//if
    }//for
    for (auto i = ret_lines->begin(); i != ret_lines->end(); ++i)
    {
        cout<<(*i)<<" ";
    }
    
}//for

