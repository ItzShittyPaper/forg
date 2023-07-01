#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "dirent.h" 
#include "string.h"

#include "sys/types.h"
#include "sys/stat.h"
#include "errno.h"
#include "unistd.h"
#include "fcntl.h"
#include "getopt.h"

void showhelp() {

	printf("forg - a *NIX file organizer tool\n\nUsage: forg [OPTIONS] [DIRECTORY]\nExample: forg -n flower ~/Photos\n\noptions:\n    h - show this help page\n    v - verbose\n    t - sort by file type\n    n - sort by file name\n\nwritten by m4kulatura\nhttps://makulaturka.tk\n\n");

}

/*
ACTUAL CODE STARTS HERE
*/

struct flags {

	int recursive;
	int verbose;

} flags;

const char* format[] = {
	"txt", 
	"md", 
	"rtf", 
	"docx", 
	"doc", 
	"pdf", 
	"epub", 
	"webm", 
	"mp4", 
	"avi",  
	"mkv",  
	"mov",  
	"ogv",  
	"flac", 
	"mp3", 
	"aiff",
	"aac", 
	"ogg",  
	"opus",
	"m4a",  
	"wav",  
	"wv", 
	"ape",  
	"png",  
	"jpg",  
	"gif", 
	"bmp", 
	"tga",
	"tiff"
};

const int id[] = {
	0, 
	0, 
	0, 
	0,
	0, 
	0, 
	0, 
	1, 
	1, 
	1,  
	1,  
	1,  
	1,  
	2, 
	2, 
	2,
	2, 
	2,  
	2,
	2,  
	2,  
	2, 
	2,  
	3,  
	3,  
	3, 
	3, 
	3,
	3
};


int opt;
char d_path[255]; // here I am using sprintf which is safer than strcat

void make_type_sort_folder(char* current_dir, char* format) {
	/* the arg should be the current forg directory: argv[optind] */
	
	char* string = (char*)malloc(4096 * sizeof(char*));
	snprintf(string, 4096, "%s/%s", current_dir, format);
	if (mkdir(string, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
		printf("Error: %s\n", strerror(errno));
	}
	free(string);
}

void show_dir_content(char * path, int type) {
	
	char *path_buf = (char*)malloc(4096 * sizeof(char*));
	char *move_buf = (char*)malloc(4096 * sizeof(char*));
	int check;

	DIR * d = opendir(path); // open the path

	if(d == NULL) return; // if was not able, return
	struct dirent * dir; // for the directory entries

	while ((dir = readdir(d)) != NULL) {

		if(dir-> d_type != DT_DIR) { // if the type is not directory just print it with blue color

			sprintf(path_buf, "%s%s", d_path, dir->d_name);
			chdir(path);
				
			switch(type) {
				
				case 0: if (strstr(path_buf, path) != NULL) {

						for(int i; format[i] != NULL; i++) {

							switch(id[i]) {

								/* check if a folder exists, then move the file to that folder. if it doesn't exist, make the folder first. */

								case 0:
									make_type_sort_folder(".", "documents");
									sprintf(move_buf, "%s %s %s/", "mv", path_buf, "documents");
									popen(move_buf, "r");
									break;
								case 1:
									make_type_sort_folder(".", "movies");
									sprintf(move_buf, "%s %s %s/", "mv", path_buf, "movies");
									popen(move_buf, "r");
									break;
								case 2:
									make_type_sort_folder(".", "audio");
									sprintf(move_buf, "%s %s %s/", "mv", path_buf, "audio");
									popen(move_buf, "r");
									break;
								case 3:
									make_type_sort_folder(".", "photo");
									sprintf(move_buf, "%s %s %s/", "mv", path_buf, "photo");
									popen(move_buf, "r");
									break;
								default:
									make_type_sort_folder(".", "other");
									sprintf(move_buf, "%s %s %s/", "mv", path_buf, "other");
									popen(move_buf, "r");
									break;



							}

							if (flags.verbose != 0)
								printf("\nPATHBUF: %s\nCMDBUF (movebuf): %s\n", path_buf, move_buf);

							i = 0; break;

						}
				
						} break;
					case 1:

							if (strstr(path_buf, optarg) != NULL) {

								make_type_sort_folder(".", optarg);
								sprintf(move_buf, "%s %s %s/", "mv", path_buf, optarg);
								popen(move_buf, "r");
								

							}

							if (flags.verbose != 0)
								printf("\nPATHBUF: %s\nCMDBUF (movebuf): %s\nOPTARG: %s\n", path_buf, move_buf, optarg);


						break;
				}
				chdir("../");

		} else if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 && flags.recursive > 0) { // if it is a directory

			printf("%s\n", dir->d_name); // print its name in green
			sprintf(d_path, "%s/%s", path, dir->d_name);

			show_dir_content(d_path, type); // recall with the new path

		}
	}

	free(path_buf); 
	closedir(d); // finally close the directory
}

int main(int argc, char *argv[]) {

	// put ':' in the starting of the
	// string so that program can 
	//distinguish between '?' and ':' 

	while((opt = getopt(argc, argv, ":rvtn:h?")) != -1) {

		switch(opt)  {

			case 't':
				show_dir_content(argv[optind], 0);
				break;
			case 'n':
				show_dir_content(argv[optind], 1);
				break;
			case 'r':
				flags.recursive = 1;
				break;
			case 'v':
				flags.verbose = 1;
				break;
			case 'h':
				showhelp();
				return 0;
				break;
			case '?':
				printf("Unknown option: %c\n", optopt);
				break;
			case ':':
				printf("Missing arg for %c\n", optopt);
				break;
		} 
	} 

	//make_type_sort_folder(argv[optind], "webm");

	// optind is for the extra arguments
	// which are not parsed
	for(int index = optind; index < argc; index++ && flags.verbose != 0){     
		printf("extra arguments: %s\n", argv[optind]);
	}
     
	return 0;

}
