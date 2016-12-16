#include <iostream>
#include <istream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#include <fstream>
#include <list>
#include <ctype.h>



//comparison-op-->=|<
string comparison_op(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{
    ofstream  out;
    out.open("parser_output.txt");
    string check;
    string exp;
    check=*(*o);
    if(check=="<")
    {
        out<<"comparison_op(<)\n";
        return("comparison_op(<)\n");
    }
    else if (check=="=")
    {
        out<<"comparison_op(=)\n";
        return("comparison_op(=)\n");
    }
    else return("error");

}

// factor-->(exp)|number|identifier
string exp(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to);
string factor(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{

    ofstream  out;
    out.open("parser_output.txt");
    string check;
    string ex;

    check=*(*o);
    if (check=="(")
    {
        *(*o)++;
        ex=exp(output,o,tokens,to);
        *(*o)++;
        out<<"("+ex+")\n";
        return("("+ex+")\n");
    }
    else if(check=="number")
    {
        *(*to)++;
        string tok=*(*to);
        out<<check+"("+tok+")"+"n";
        return(check+"("+tok+")"+"\n");
    }
    else if(check=="identifier")
    {
        *(*to)++;
        string tok=*(*to);
        out<<check+"("+tok+")"+"n";
        return(check+"("+tok+")"+"\n");
    }
    else return("error");
}


//mulop--->*|/
string mulop(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{
    ofstream  out;
    out.open("parser_output.txt");
    string check;
    string exp;
    check=*(*o);
    if(check=="*")
    {
        out<<"mul_op(*)\n";
        return("mul_op(*)\n");
    }
    else if (check=="/")
    {
        out<<"mul_op(/)\n";
        return("mul_op(/)\n");
    }
    else return("error");

}

//term-->factor {mulop factor}
string term(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{
    ofstream  out;
    string f2;
    out.open("parser_output.txt");

    string check;
    string f= factor(output,o,tokens,to);
    if(f!="error")
        *(*o)++;
    string m=mulop(output,o,tokens,to);
    if(m!="error")
    {
        while(m!="error")
        {
            *(*o)++;
            f2=factor(output,o,tokens,to);
            if(f2!="error")
            {
                f=m+f+f2;
                *(*o)++;
                m=mulop(output,o,tokens,to);
            }
        }
        *(*o)--;
        return(f+"\n");
        out<<f+"\n";
    }
    else
    {
        *(*o)--;
        return(f+"\n");
        out<<f+"\n";
    }

}

//addop-->+|-
string addop(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{
    ofstream  out;
    out.open("parser_output.txt");
    string check;
    string exp;
    check=*(*o);
    if(check=="+")
    {
        out<<"add_op(+)\n";
        return("add_op(+)\n");
    }
    else if (check=="-")
    {
        out<<"add_op(-)\n";
        return("add_op(-)\n");
    }
    else return("error");

}

//simple-exp-->term {addop term}
string simple_exp(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{
    ofstream  out;
    string f2;
    out.open("parser_output.txt");

    string f= term(output,o,tokens,to);
    if(f!="error")
        *(*o)++;
    string m=addop(output,o,tokens,to);
    if(m!="error")
    {
        while(m!="error")
        {
            *(*o)++;
            f2=term(output,o,tokens,to);
            if(f2!="error")
            {
                f=m+f+f2;
                *(*o)++;
                m=mulop(output,o,tokens,to);
            }
        }
        *(*o)--;
        return(f+"\n");
        out<<f+"\n";
    }
    else
    {
        *(*o)--;
        return(f+"\n");
        out<<f+"\n";
    }

}

//exp--->simple-exp  comparison-op simple-exp |simple-exp
string exp(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{
    ofstream  out;
    out.open("parser_output.txt");
    string exp,c,exp2,e;
    exp=simple_exp(output,o,tokens,to);
    *(*o)++;;
    c=comparison_op(output,o,tokens,to);
    if(c!="error")
    {
        *(*o)++;
        exp2=simple_exp(output,o,tokens,to);
        e=c+exp+exp2;
        out<<e;
        return(e+"\n");
        out<<e+"\n";
    }
    else
    {
        *(*o)--;
        return(exp+"\n");
        out<<exp+"\n";
    }

}

//write stmt--->write exp
string write_stmt(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{
    ofstream  out;
    out.open("parser_output.txt");
    string wr="write";
    *(*o)++;
    string ex=exp(output,o,tokens,to);
    out<<wr+"\n"+ex+"\n";
    return(wr+"\n"+ex+"\n");

}

//read stmt-->read identifier
string read_stmt(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{

    *(*o)++;
    ofstream out;
    out.open("parser_output.txt");
    string re="read";
    string tok=*(*to);
    out<<re+*(*o)+tok+"\n";
    return("read identifier("+tok+")\n");


}
//assign-stmt-->identifier := exp
string assign_stmt( list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{

    *(*to)++;
    string id=*(*to);
    ofstream  out;
    out.open("parser_output.txt");
    *(*o)++;
    string ex=exp(output,o,tokens,to);
    out<<" identifier "+id+":= \n"+ex+"\n";
    return( "identifier "+id+":= \n"+ ex+"\n");
}
string if_stmt(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to);
string repeat_stmt(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to);
//statment--<if-stmt|write-stmt|read-stmt|assign-stmt|repeat-stmt
string statment(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{
    ofstream  out;
    out.open("parser_output.txt");
    *(*o)++;
    if(*(*o)==":=")
    {
        string a=  assign_stmt(output,o,tokens,to);
        return("assign_stmt\n\n"+a+"\n");
    }
    *(*o)--;
    if(*(*o)=="untill")
    {
        return("error");
    }
    else if(*(*o)=="if")
    {
        string i=if_stmt(output,o,tokens,to);
        return("\n"+i+"\n");
    }
    else  if(*(*o)=="read")
    {
        string r=read_stmt(output,o,tokens,to);
        return("\n"+r+"\n");
    }
    else if(*(*o)=="write")
    {
        string w= write_stmt(output,o,tokens,to);
        return("\n"+w+"\n");
    }

    else if(*(*o)=="repeat")
    {
        string g= repeat_stmt(output,o,tokens,to);
        return("\n"+g+"\n");
    }
    else return("error");
    out<<"error";
}

//stmt-sequence--> statment {;statment}
string stmt_sequence(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{
    ofstream  out;
    out.open("parser_output.txt");
    string f2,f;

    f= statment(output,o,tokens,to);
    *(*o)++;
    if(*(*o)==";")
    {
        while(*(*o)==";")
        {
            *(*o)++;
            f2=statment(output,o,tokens,to);
            if(f2=="error")
                goto ex;
            f=f+f2;
            *(*o)++;
        }
        return(f+"\n");
        out<<f+"\n";
    }
    else
    {
        *(*o)--;
        return(f+"\n");
        out<<f+"\n";
    }
ex:*(*o)--;
    return(f+"\n");
    out<<f+"\n";

}

//if-stmt-->if exp then stmt-sequence [else stmt-sequence]end
string if_stmt(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{

    fstream  out;
    string ex2;
    out.open("parser_output.txt");
    (*o)++;
    string ex=exp(output,o,tokens,to);

    *(*o)++;
    *(*o)++;
    string st=stmt_sequence(output,o,tokens,to);
    string f="if\n"+ ex +"\n"+ st;
    *(*o)--;
    if(*(*o)=="else")
    {
        ex2=stmt_sequence(output,o,tokens,to);
        f=f+ex2+"\n";
        out<<f+"\n";
        return(f+"\n");
    }
    f=f +"\n";
    out<<f+"\nend_if\n";
    return(f+"\nend_if\n");

}
//repeat-stmt--->repeat stmt-sequence unttill exp
string repeat_stmt(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{
    fstream  out;
    out.open("parser_output.txt");
    string ex2;
    *(*o)++;
    string st=stmt_sequence(output,o,tokens,to);
    *(*o)++;
    string ex=exp(output,o,tokens,to);
    ex2="repeat\n\n"+ st +"\nrepeat_cond\n"+ ex+"\nend_repeat";
    out<<ex2+"\n";
    return(ex2+"\n");

}


//program--->stmt-sequence
string program(list<string>*output,list<string>::iterator*o,list<string>*tokens,list<string>::iterator*to)
{
    ofstream out;
    out.open("parser_output.txt");
    string st=stmt_sequence(output,o,tokens,to);
    out<<st+"\n";
    return(st+"\n");
}


int run()
{

    fstream  out;
    out.open("parser_output.txt");

    //parser
    list<string>input ;
    list<string>output ;
    list<string>tokens ;
    list<string>::iterator m;
    list<string>::iterator o;
    list<string>::iterator to;
    string test;
    string identifier;
    string number;

    //to read from text file
    int pos=0;
    string x;
    ifstream ex("try.txt");
label:	if(getline(ex,x))
    {
        x.erase(x.find_last_not_of(" \n\r\t")+1);
        pos=0;
        while(pos!=x.length())
        {
            string c=x.substr(pos,1);
            input.push_back(c);
            pos++;
        }
        goto label;
    }

    //start parsing string
    for (m=input.begin();m!=input.end();++m)
    {
start:		 string check=*m;

        if (check=="+")
        {
            output.push_back(check);
        }
        else if (check=="-")
        {
            output.push_back(check);

        }
        else if (check=="*")
        {

            output.push_back(check);

        }
        else if (check=="/")
        {

            output.push_back(check);

        }
        else if (check=="=")
        {

            output.push_back(check);

        }
        else if (check=="<")
        {

            output.push_back(check);

        }
        else if (check=="(")
        {

            output.push_back(check);

        }
        else if (check==")")
        {

            output.push_back(check);

        }
        else if (check==":")
        {
            m++;
            if(*m=="=")

                output.push_back(":=");

        }
        else if (check==";")
        {

            output.push_back(check);

        }

        else if(check=="1" || check=="0"|| check=="2"|| check=="3"|| check=="4"|| check=="5"|| check=="6"|| check=="7"|| check=="8"|| check=="9")
        {
            m++;
            string ch=*m;
            number+=check;
            if(ch!="1" && ch!="0"&& ch!="2"&& ch!="3"&& ch!="4"&& ch!="5"&& ch!="6"&& ch!="7"&& ch!="8"&& ch!="9")
            {
                output.push_back("number");
                tokens.push_back(number);
                number="";
            }
            m--;
        }
        else if(check=="a"||check=="b"||check=="c"||check=="d"||check=="e"||check=="f"||check=="g"||check=="h"||check=="i"||check=="j"||check=="k"||check=="l"||check=="m"||check=="n"||check=="o"||check=="p"||check=="q"||check=="r"||check=="s"||check=="t"||check=="u"||check=="v"||check=="w"||check=="x"||check=="y" )
        {
            string ch;
            string k=input.back();
            if(check!=k)
            {
                m++;
                ch=*m;
                m--;
            }
            identifier=identifier+check;

            if(identifier=="if"||identifier=="else"||identifier=="then"||identifier=="end"||identifier=="repeat"||identifier=="until"||identifier=="read"||identifier=="write"||identifier=="Description")
            {

                output.push_back(identifier);
                if(identifier=="end")
                {
                    output.push_back(identifier);
                    identifier="";
                    break;
                }
                identifier="";
            }
            else if(ch!="a"&&ch!="b"&&ch!="c"&&ch!="d"&&ch!="e"&&ch!="f"&&ch!="g"&&ch!="h"&&ch!="i"&&ch!="j"&&ch!="k"&&ch!="l"&&ch!="m"&&ch!="n"&&ch!="o"&&ch!="p"&&ch!="q"&&ch!="r"&&ch!="s"&&ch!="t"&&ch!="u"&&ch!="v"&&ch!="w"&&ch!="x"&&ch!="y" )
            {

                output.push_back("identifier");
                tokens.push_back(identifier);
                identifier="";
            }
        }


        //to delete comment
        else if (check[0]=='{')
        {
            while(1)
            {
                m++;
                string c=*m;
                if(c=="}")
                {
                    m++;
                    goto start;
                }
            }
        }
    }

    to=tokens.begin();
    o=output.begin();
    string c=program(&output,&o,&tokens,&to);
    cout<<c;
    return 0;
}



