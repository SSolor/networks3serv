//sebastian solorzano, Ryan Hackbart -- compnet assgmt3 group 13 -- CSCN71020 25F 
//file stuff, linked list stuff (impl)

#include "FilePost.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//this is all fundamental linked list stuff; it should need no explanation
void addPost(PPOSTNODE* head, char msg[POST_MAX]){
    PPOSTNODE newnode = (PPOSTNODE)malloc(sizeof(POSTNODE));
    if(newnode==NULL){
        fprintf(stderr,"no mem :( (linked list creation)\n)");
        exit(EXIT_FAILURE);
            //this probably isn't very nice to the client, but what else can I do?
    }
    else{
        strncpy(newnode->post,msg,sizeof(newnode->post));
        newnode->next=*head;

        *head=newnode;
    }
}

void pop(PPOSTNODE* head, char out[POST_MAX]){
    PPOSTNODE current = *head;
    if(current!=NULL){
        strncpy(out,current->post,POST_MAX);
        *head=current->next;
        free(current);
    }
    return;
}

void peekDepth(PPOSTNODE head, int depth, char out[POST_MAX]){
    PPOSTNODE current = head;

    for(int i=0; i<depth;i++){
        if(current!=NULL){
            //reaching requested depth
            current=current->next;
        }
        else{
            fprintf(stderr,"peek out of bounds\n");
            exit(EXIT_FAILURE);
            //this probably isn't very nice to the client, but what else can I do?
        }
    }
    //now that we've reached the proper depth
    strncpy(out,current->post,POST_MAX);
    return;
}

int countListLength(PPOSTNODE head){
    int len=0;
    PPOSTNODE current =head;

    while(current!=NULL){
        len++;
        current=current->next;
    }

    fprintf(DEBUG,"list length is %d\n",len);
    return len;
}

void deleteList(PPOSTNODE* head);


void loadPostsFromFile(PPOSTNODE* head, char filename[]){
    char buf[POST_MAX];
    FILE* fp = fopen(filename,"r");
		//huh, thought this didn't work on linux
		//we could use write() and fds instead, but I don't see the point?
	if(fp!=NULL){

		while(fgets(buf,POST_MAX,fp)!=NULL){//fgets can return this apparently, so convenient
            //if client uses scanf to generate posts:
            // buf[strcspn(buf,"\n")]='\0';
                //funny little function that'll give the index of the first \n, letting us clip it
            fprintf(DEBUG,"loaded once\n");

			addPost(head,buf);
		}
		fclose(fp); //seg faults if it was null
	}
	//doesn't actually matter if file is empty or doesn't exist
	//a new one will be made upon exiting the program
    return;
}

void savePostsToFile(PPOSTNODE* head, char filename[]){
    char buf[POST_MAX];
    FILE* fp = fopen(filename,"w");
    //dont bother checking for null since this creates a file anyways
    while(*head!=NULL){
        fprintf(DEBUG,"saved once\n");

        pop(head,buf); //conveniently, this also serves the role of emptying the list

        //if client uses scanf to generate posts:
        //fprintf(fp,"%s\n",buf); 
        //if client uses fgets to generate posts:
        fprintf(fp,"%s",buf); 

    }
    fclose(fp);
    return;
}