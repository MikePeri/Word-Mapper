#include "Query.h"
#include "TextQuery.h"
#include <memory>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <regex>
#include <string>
#include <cmath>    
using namespace std;

shared_ptr<QueryBase> QueryBase::factory(const string& s)
{
    std::shared_ptr<std::vector<std::string>> words;
    istringstream in(s);
    string word;
    while (in >> word)
    {
        if(!words)
            words.reset(new vector<string>);
        words->push_back(word);    
    }//while
    if(words->size()==3 || words->size()==1)
    {
        string first_word = words->at(0);
        if(first_word.compare("AND") == 0 && words->size() == 3)
        {
            string left = words->at(1);
            string right = words->at(2);
            return std::shared_ptr<QueryBase>(new AndQuery(left,right));
        }//else if
        else if(first_word.compare("OR") == 0 && words->size() == 3)
        {
            string left = words->at(1);
            string right = words->at(2);
            return std::shared_ptr<QueryBase>(new OrQuery(left,right));
        }//else if
        else if(first_word.compare("AD") == 0 && words->size() == 3)
        {
            string left = words->at(1);
            string right = words->at(2);
            return std::shared_ptr<QueryBase>(new AdjacentQuery(left,right));
        }//else if 
        else if(words->size() == 1) //Only one Word
        {
            return std::shared_ptr<QueryBase>(new WordQuery(first_word));
        }//ese
        else
             throw std::invalid_argument( "Unrecognized search\n" );
    }//if
    else
    {
        throw std::invalid_argument( "Unrecognized search\n" );
    }//else
    
}//facroty
QueryResult AndQuery::eval (const TextQuery& text) const
{
    QueryResult left_result = text.query(left_query);
    QueryResult right_result = text.query(right_query);
    auto ret_lines = make_shared<set<line_no>>();
    set_intersection(left_result.begin(), left_result.end(),
        right_result.begin(), right_result.end(), 
        inserter(*ret_lines, ret_lines->begin()));
   return QueryResult(rep(), ret_lines, left_result.get_file());
}

QueryResult OrQuery::eval(const TextQuery &text) const
{
    QueryResult left_result = text.query(left_query);
    QueryResult right_result = text.query(right_query);
    auto ret_lines = make_shared<set<line_no>>(left_result.begin(), left_result.end());
    ret_lines->insert(right_result.begin(), right_result.end());
    return QueryResult(rep(), ret_lines, left_result.get_file());
}

QueryResult AdjacentQuery::eval (const TextQuery& text) const
{
    QueryResult left_result = text.query(left_query);
    QueryResult right_result = text.query(right_query);
    auto ret_lines = make_shared<set<line_no>>();
    set<line_no>::iterator it;
    for (auto i = left_result.begin(); i != left_result.end(); ++i)
    {
        int value = (*i);
        it = find(right_result.begin(),right_result.end(),value+1);
        if(it != right_result.end())
            {
                ret_lines->insert(value);
            }//if
        it = find(right_result.begin(),right_result.end(),value-1);
        if(it != right_result.end())
            {
                ret_lines->insert(value-1);
            }//if
    }//for
    return QueryResult(rep(), ret_lines, left_result.get_file());
}//eval

std::ostream &print(std::ostream &os, const QueryResult &qr)
{
    std::shared_ptr<std::vector<std::string>> words;
    string query = qr.sought;
    istringstream in(query);
    string word;
    while (in >> word)
    {
        if(!words)
            words.reset(new vector<string>);
        words->push_back(word);    
    }//while

    if(words->at(0).compare("AD") != 0)
    {
         os << "\"" << qr.sought << "\"" << " occurs " << 
            qr.lines->size() << " times:" <<std::endl;
            for (auto num : *qr.lines)
            {
                    os << "\t(line " << num + 1 << ") " 
                        << *(qr.file->begin() + num) << std::endl;
            }
    }//if Not AD operation
       
    else //Case of AD operation
    {
         os << "\"" << qr.sought << "\"" << " occurs " << 
             qr.lines->size() << " times:" <<std::endl;
        int counter = 0;
        for (auto num : *qr.lines)
            {
                counter++;
                    if(counter <= qr.lines->size()-1)
                        os << "\t(line " << num + 1 << ") " 
                            << *(qr.file->begin() + num) << "\n"
                        << "\t(line " << num + 2 << ") " 
                            << *(qr.file->begin() + num+1) << "\n" << endl;
                    else 
                        os << "\t(line " << num + 1 << ") " 
                            << *(qr.file->begin() + num) << "\n"
                        << "\t(line " << num + 2 << ") " 
                            << *(qr.file->begin() + num+1) << "\n";
                    
            }//for
    }//else
    
    
    return os;
}

