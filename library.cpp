/* complete this file */
#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  // needed for open
#include <sys/stat.h>   // needed for open
#include <fcntl.h>      // needed for open
#include <unistd.h>     // needed for read and write

//methods for Document
void Document::updateTitle(const char *newTitle){
    strcpy(_title,newTitle);
}

void Document::updateYear(int newYear){
    _year = newYear;
}

void Document::updateQuantity(int newQuantity){
    _quantity = newQuantity;
}

char *Document::getTitle(){
    return _title;
}

int Document::getYear(){
    return _year;
}

int Document::getQuantity(){
    return _quantity;
}

int Document::borrowDoc(){
    if(_quantity>0){
      _quantity=_quantity-1;
      return 0;
    }
    else{
      return -1;
    }
}

void Document::returnDoc(){
  _quantity=_quantity+1;
}

//methods for Novel
Novel::Novel(const char *title, const char *author, int year, int quantity){
    _title = new char[strlen(title)+1];
    strcpy(_title, title);
    _author=new char[strlen(author)+1];
    strcpy(_author, author);
    _year = year;
    _quantity = quantity;
}


Novel::~Novel(){
    delete[] _title;
    delete[] _author;
    printf("destructor called\n");
}

document_type Novel::getDocType(){
    return DOC_NOVEL;
}

void Novel::print(){
    printf("Novel, title: %s, author: %s, year: %d, quantity: %d\n",_title,_author,_year, _quantity);
}

void Novel::updateAuthor(const char *newAuthor){
    strcpy(_author, newAuthor);
}

char *Novel::getAuthor(){
    return _author;
}

//Methods for Comics
Comic::Comic(const char *title, const char *author, int issue, int year, int quantity){

    _title = new char[strlen(title)+1];
    strcpy(_title, title);
    _author=new char[strlen(author)+1];
    strcpy(_author, author);
    _issue=issue;
    _year=year;
    _quantity=quantity;
}

Comic::~Comic(){
    delete[] _title;
    delete[] _author;
    printf("destructor called\n");
}

document_type Comic::getDocType(){
    return DOC_COMIC;
}

void Comic::print(){
    printf("Comic, title: %s, author: %s, issue: %d, year: %d, quantity: %d\n",_title, _author, _issue, _year, _quantity);
}

void Comic::updateAuthor(const char *newAuthor){
    strcpy(_author, newAuthor);
}

void Comic::updateIssue(int newIssue){
    _issue = newIssue;
}

char *Comic::getAuthor(){
    return _author;
}

int Comic::getIssue(){
  return _issue;
}

//methods for magazine
Magazine::Magazine(const char *title, int issue, int year, int quantity){
    _title = new char[strlen(title)+1];
    strcpy(_title, title);
    _issue = issue;
    _year = year;
    _quantity = quantity;
}

Magazine::~Magazine(){
    delete[] _title;
    printf("destructor called\n");
}

document_type Magazine::getDocType(){
    return DOC_MAGAZINE;
}

void Magazine::print(){
    printf("Magazine, title: %s, issue: %d, year: %d, quantity: %d\n",_title, _issue, _year, _quantity);
}

void Magazine::updateIssue(int newIssue){
    _issue = newIssue;
}

int Magazine::getIssue(){
  return _issue;
}

//methods for Library
Library::Library(){}

void Library::print(){
    for(int i=0; i<_docs.size();i++){
        _docs[i]->print();
    }
}

int Library::dumpCSV(const char *filename){
    int fd1 = open(filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    char *buffer;
    if(fd1 == -1) {
        printf("error with open\n");
        return -1;
    }
    for(int i=0; i<_docs.size();i++){
      if(_docs[i]->getDocType()==DOC_NOVEL){
          Novel* novel;
          auto it=_docs.begin()+i;
          novel = static_cast<Novel*>(*it);
          char type[] = "novel";
          char *title=new char[strlen(novel->getTitle())+1];
          strcpy(title, novel->getTitle());
          char *author=new char[strlen(novel->getAuthor())+1];
          strcpy(author,novel->getAuthor());
          int year = _docs[i]->getYear();
          int quantity = _docs[i]->getQuantity();
          buffer = new char[strlen(type)+strlen(title)+strlen(author)+sizeof(year)+sizeof(quantity)];
          sprintf(buffer,"%s,%s,%s,,%d,%d\n",type,title,author,year,quantity);
          write(fd1,buffer,strlen(buffer));
      }
      else if(_docs[i]->getDocType()==DOC_MAGAZINE){
          Magazine* magazine;
          auto it=_docs.begin()+i;
          magazine = static_cast<Magazine*>(*it);
          char type[] = "magazine";
          char *title=new char[strlen(magazine->getTitle())+1];
          strcpy(title, magazine->getTitle());
          int issue = magazine->getIssue();
          int year = _docs[i]->getYear();
          int quantity = _docs[i]->getQuantity();
          buffer = new char[strlen(type)+strlen(title)+sizeof(issue)+sizeof(year)+sizeof(quantity)];
          sprintf(buffer,"%s,%s,,%d,%d,%d\n",type,title,issue,year,quantity);
          write(fd1,buffer,strlen(buffer));
      }
      else{
          Comic* comic;
          auto it=_docs.begin()+i;
          comic = static_cast<Comic*>(*it);
          char type[] = "comic";
          char *title=new char[strlen(comic->getTitle())+1];
          strcpy(title, comic->getTitle());
          char *author=new char[strlen(comic->getAuthor())+1];
          strcpy(author,comic->getAuthor());
          int issue = comic->getIssue();
          int year = _docs[i]->getYear();
          int quantity = _docs[i]->getQuantity();
          buffer = new char[strlen(type)+strlen(title)+strlen(author)+sizeof(issue)+sizeof(year)+sizeof(quantity)];
          sprintf(buffer,"%s,%s,%s,%d,%d,%d\n",type,title,author,issue,year,quantity);
          write(fd1,buffer,strlen(buffer));
      }

    }


    close(fd1);
    return 0;
}

Document *Library::searchDocument(const char *title){
    for(int i=0; i<_docs.size();i++){
        if(!strcmp(_docs[i]->getTitle(),title)){
            return _docs[i];
        }
    }
    return NULL;
}

int Library::addDocument(Document *d){
    _docs.push_back(d);
    return 0;
}

int Library::delDocument(const char *title){
    for(int i=0; i<_docs.size();i++){
        if(!strcmp(_docs[i]->getTitle(),title)){
            _docs.erase(_docs.begin()+i);
            return 0;
        }
    }
    return -1;
}

int Library::countDocumentOfType(document_type t){
    int count=0;
    for(int i=0;i<_docs.size();i++){
        if(_docs[i]->getDocType()==t){
            count++;
        }
    }
    return count;
}

int Library::borrowDoc(const char *title){
    for(int i=0;i<_docs.size();i++){
        if(!strcmp(_docs[i]->getTitle(),title)&&_docs[i]->getQuantity()>0){
            _docs[i]->updateQuantity(_docs[i]->getQuantity()-1);
            return 0;
        }
    }
    return -1;
}

int Library::returnDoc(const char *title){
  for(int i=0;i<_docs.size();i++){
      if(!strcmp(_docs[i]->getTitle(),title)){
          _docs[i]->updateQuantity(_docs[i]->getQuantity()+1);
          return 0;
      }
  }
  return -1;
}
