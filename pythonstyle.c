/*
This program is converting c programs which are partly written in python style.
In python style the indent is used to define blocks instead of { and }
a sections that must be converted starts with the keyword 'pythonstyle' on the first column.
If you add a ':' you can leave out the semicolons in your python styled section.
All functions in this program are writhen in python style themselves

In python styled sections only double slash comments are allowed. The must indented the same as the program
*/




#include <stdio.h>
#include <string.h>

#define bool unsigned char
#define true 1
#define false 0
#define txtSKIPEXAMP
#include "milkstrings.c"

#define forloop(x,y,z) for (x = (y);x <= (z);x++) 


int testswitch(int y) {
    // test the switch statement;
    int i;
    int x;
    for (i=0 ; i < 5 ;i++) {
        x = y;
        switch(i) {
            case 0:
            case 1: {
                x++; }
            break;
            case 2 : {
                x = 4*x; }
            break; }
        printf("%d %d\n",i,x); }
    return x; }



FILE * fglob1, *fglob2,*fglob3 ;


bool readstringfrom(tXt fname,tXt * ps ,FILE ** pfi)  {
    // read a line of text from a file
    if (*pfi == NULL) {
        *pfi = fopen(fname,"r") ; }
    if (*pfi != NULL) {
        char inbuf[txtMAXLEN] ;
        inbuf[0] = 0 ;
        if (fgets(inbuf,txtMAXLEN,*pfi) == NULL)  {
            fclose(*pfi) ;
            *pfi = NULL ; }
        else  {
            int li = strlen(inbuf) ; 
            if (li > 0 && inbuf[li-1]=='\n')  {
                inbuf[li-1]=0 ;  }
            *ps =  txtConcat(inbuf,NULL) ;  }}
    return (*pfi != NULL ) ; }


void writestringto(tXt fname,tXt s , FILE ** pfo)  {
    //write string to a file
    if (*pfo == NULL) {
        *pfo = fopen(fname,"w+") ; }
    if (*pfo != NULL) {
        if (!s[0]) {
            fprintf(*pfo,"\n") ; }
        else {
            fprintf(*pfo,"%s\n",s) ;  }}}


void finishfile(FILE ** pf)  {
    //close a file that was written to
    if (*pf != NULL) {
        fclose(*pf) ; }
    *pf = NULL ;  }

bool FileExists(tXt nam)  {
    // test wether file exists
    bool rslt = true ;
    FILE * fi = fopen(nam,"r") ;
    if (fi == NULL) {
        rslt = false ; }
    else {
        fclose(fi) ; }
    return rslt ;  }


int leadspaces(tXt s)  {
    //count the numbber of leading spaces in astring;
    int rslt = 0 ;
    while (*s == ' ')  {
        s++ ;
        rslt++ ; }
    return rslt  ;
    ; }

bool inset(char c,tXt st)  {
    //check if char is in some ranges of chars 
    int i = 0 ;
    bool found = false ;
    while (i < strlen(st) && !found)  {
        found = (c >= st[i] && c <= st[i+1]) ;
        i += 2 ;  }
    return found ; }


tXt codeOnly(tXt s)  {
    //remove parts of strings that are within quotes;
    bool inquotes = false ;
    s = txtConcat(s,NULL) ;
    int i ;
    bool escaped = false ;
    forloop(i,0,strlen(s)-1) {
        if (s[i] == '\"' && !escaped) {
            inquotes = !inquotes ; }
        else {
            escaped = (s[i] == '\\')  ;
            if (inquotes) {
                s[i] = '.' ; }}}
    return s ; }




int main(int argc, char * argv[])  {
    // convert a file with python style sections (*.cpy) to a regular c file;
    tXt s = " ";
    tXt s2 = "  " ;
    bool instyle = false ;
    int linum = 0 ;
    bool firstpylin = false ;
    bool stufcolon = false ;
    if (argc < 2)  {
        printf("no project\n") ;
        return 2 ; }
    tXt finame = fridge(txtConcat(argv[1],".cpy",NULL)) ;
    if (!FileExists(finame))  {
        printf("no file %s\n",finame) ;
        return 1 ; }
    tXt foname = fridge(txtConcat(argv[1],".c",NULL)) ;
    while (readstringfrom(finame,&s,&fglob1))  {
        linum++ ;
        if (txtPos(s,"pythonstyle") == 0)  {
            instyle = true ;
            stufcolon = (s[11] == ':') ;
            firstpylin = true ;
            s = txtSub(s,strlen("pythonstyle ")+(stufcolon?1:0),999) ; }
        else {
            if (s2[0] == 0 && !firstpylin) {
                instyle = false ; }
            firstpylin = false ; }
        if (instyle)  {
            if (s[0] && inset(s[strlen(s)-1],"{{}}")) {
                printf("warning %c in line %d\n",s[strlen(s)-1],linum) ; }
            int indent = leadspaces(s) ;
            if ((indent % 4) != 0) {// indents must be multiples of 4 
                printf("invalid indent %d at line %d:%s\n",indent,linum,txtTrim(s)) ; }
            int dif = indent-leadspaces(s2) ;
            if (stufcolon && dif <= 0 && s2[0] &&(s2[strlen(s2)-1] != ':')) {
                s2 = txtConcat(s2,";",NULL) ; }
            if (dif != 0) {
                int ipos = (txtPos(s2,"//")==txtNOTFOUND)?9999:txtPos(codeOnly(s2),"//") ;
                s2 = txtConcat(s2," ",NULL) ;
                if (dif > 0) {
                    s2 = txtInsert(s2,txtFill('{',dif/4),ipos) ; }
                else {
                    s2 = txtInsert(s2,txtFill('}',-dif/4),ipos) ;; }}}
        if (linum > 1) {
            writestringto(foname,s2,&fglob2) ; }
        s2 = s ; }
    writestringto(foname,s2,&fglob2) ;
    finishfile(&fglob2) ;
    return 0 ; }


