//sebastian solorzano, Ryan Hackbart -- compnet assgmt3 group 13 -- CSCN71020 25F 
//file stuff, parsing stuff (intf)

//okay so having all the functions in one file was so unbelievably ugly and I couldn't stand it
//and I figured out how to do multiple files with gcc so its fine
#pragma once

#include <stdio.h>
#include <string.h>
#include <unistd.h>

	//char limits
#define TOPIC_AUTH_MAX 64
#define BODY_MAX 512
#define POST_MAX TOPIC_AUTH_MAX+TOPIC_AUTH_MAX+BODY_MAX


//the reqs said all posts must be loaded into memory on startup, even if 
//I think it might be better to only load them when at the moment we need them
//as such, i'll be making a linked list here to store it all


//so it turns out I was overcomplicating things, we don't actually need to parse anything 
//on the server side at all.
//we have to send them back the way they came anyways, and its not like we have to print them here
typedef struct postlist{
	char post[POST_MAX];
	struct postlist* next;
}POSTNODE, *PPOSTNODE;
//man writing all the linked list functions is such a pain

//LINKED LIST FXNS
void addPost(PPOSTNODE* list, char msg[POST_MAX]);
void pop(PPOSTNODE* head, char out[POST_MAX]);

//do we need to remove? don't think so
int postsLength(PPOSTNODE list);
void deleteList(PPOSTNODE* list);
//we don't need read/display functions, we can literally just copy the strings


//FILE READWRTIE FXNS
void loadPostsFromFile(PPOSTNODE* head);
//POST PARSING FUNCTIONS