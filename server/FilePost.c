#include "FilePost.h"
#include <stdlib.h>
//defined by group as protocol
#define SUBSTITUTE_CHAR '\07'
//assignment said we must use ',' to split, so ',' the user enters are substituted with this char
//and then translated back to



//this is all fundamental linked list stuff; it should need no explanation
void addPost(PPOSTNODE* head, char msg[POST_MAX]){
    PPOSTNODE newnode = (PPOSTNODE)malloc(sizeof(POSTNODE));
    if(newnode==NULL){
        fprintf(stderr,"no mem :( (linked list creation)\n)");
        exit(EXIT_FAILURE);
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

int postsLength(PPOSTNODE head){
    int len=0;
    PPOSTNODE current =head;

    while(current!=NULL){
        len++;
        current=current->next;
    }

    return len;
}

void deleteList(PPOSTNODE* head);


void loadPostsFromFile(PPOSTNODE* head){
    FILE* fp = fopen("posts.dat","r");
		//huh, thought this didn't work on linux
		//we could use write() and fds instead, but I don't see the point?
	if(fp!=NULL){
		char buf[POST_MAX];
		while(fgets(buf,POST_MAX,fp)!=NULL){//fgets can return this apparently, so convenient
			addPost(head,buf);
		}
		fclose(fp); //seg fault if it was null
	}
	//doesn't actually matter if file is empty or doesn't exist
	//a new one will be made upon exiting the program
}