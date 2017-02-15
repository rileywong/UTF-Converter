#include "utfconverter.h"
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <sys/utsname.h>
#include <time.h>
#include <sys/times.h>
#include <stdio.h>
#include <inttypes.h>

char* filename;
char* fileout;
endianness source;
endianness conversion;
bool v1;
bool v2;
double realTime;
double realTime1;
double realTime2;
double userTime;
double userTime1;
double userTime2;
double sysTime;
double sysTime1;
double sysTime2;
int glyphCount;
int utfCount;


int main(int argc, char** argv)
{
	v1 = false;
	v2 = false;
	filename = NULL;
	fileout = NULL;
	glyphCount = 0;
	/* After calling parse_args(), filename and conversion should be set. */
	parse_args(argc, argv);
	int fd;
	fd = open(filename, O_RDONLY); 
	if(fd==-1){
		return EXIT_FAILURE;
	}
	unsigned int buf[4];
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 0;
	int rv;
	rv = 0;
	
	Glyph* glyph = malloc(sizeof(Glyph)); 
	


	static clock_t startRead;
	static clock_t endRead;
	static struct tms startReadT;
	static struct tms endReadT;

	static clock_t startRead1;
	static clock_t endRead1;
	static struct tms startReadT1;
	static struct tms endReadT1;

	static clock_t startRead2;
	static clock_t endRead2;
	static struct tms startReadT2;
	static struct tms endReadT2;


	

	startRead = times(&startReadT);
	startRead2 = times(&startReadT2);
	if((rv = read(fd, &buf[0], 1) == 1) && 
	   (rv = read(fd, &buf[1], 1) == 1)	){ 
	   	


		if(buf[0] == 0xff && buf[1] == 0xfe){
		
			source = LITTLE; 
			fill_glyph(glyph,buf,conversion,STDIN_FILENO);
		

		
		glyph = swap_endianness(glyph);
		

	
		write_glyph(glyph);
		} else if(buf[0] == 0xfe && buf[1] == 0xff){
			/*file is little endian*/
			source = BIG;
			fill_glyph(glyph,buf,conversion,STDIN_FILENO);
		
		glyph = swap_endianness(glyph);
		



		
		write_glyph(glyph);
		
		
		} 


		else if(buf[0] == 0xEF && buf[1]==0xBB && (buf[2] = read(fd, &buf[2], 1)) == 0xBF ){
			source = EIGHT;
			utfCount = 0;
			fill_glyph(glyph,buf,conversion,STDIN_FILENO);
			glyph = swap_endianness(glyph);
			write_glyph(glyph);
		}


			else {
			/*file has no BOM*/
			free(&glyph->bytes); 
			
			quit_converter(NO_FD); 
			return EXIT_FAILURE;

		
		 memset(glyph, 0, sizeof(Glyph));

	
	}



	/* Now deal with the rest of the bytes.*/
	startRead1 = times(&startReadT1);
	if((source == LITTLE)||(source==BIG)){
	while((rv = read(fd, &buf[0], 1) == 1 )&&  
			(rv = read(fd, &buf[1], 1) == 1)){
		fill_glyph(glyph,buf,conversion,STDIN_FILENO);
			

		glyph = swap_endianness(glyph);
		

		
		write_glyph(glyph);

		
		memset(glyph, 0, sizeof(Glyph));
	        
	}
}
else if (source == EIGHT){
	while((rv = read(fd,&buf[0],1 == 1))){
		unsigned int bite = buf[0] >> 7;
		if(bite == 0x0){
		//	printf("%s\n", "case 1");
			utfCount = 1;
			printf("%u\n", buf[0]);
				glyphCount++;

			continue;

		}
		bite = buf[0] >>5;

		printf("%u\n", bite);
		if(bite == 0x6){
		//	printf("%s\n", "case 2");
			read(fd,&buf[1],1);
			utfCount = 2;
				glyphCount++;

			continue;
		}
		bite = buf[0] >>4;


		if(bite == 0xE){
		//	printf("%s\n", "case 3");
			read(fd,&buf[1],1);
			read(fd,&buf[2],1);
			utfCount = 3;
				glyphCount++;

			continue;
		}
		bite = buf[0] >>3;


		if(bite == 0x1E){
		//	printf("%s\n", "case 4");
			read(fd,&buf[1],1);
			read(fd,&buf[2],1);
			read(fd,&buf[3],1);
			utfCount = 4;
				glyphCount++;

			continue;
		}

		printf("%s\n", "shouldnt get here");
		return EXIT_FAILURE;

	}
	/*
	while((rv = read(fd, &buf[0], 1) == 1 )&&  
			(rv = read(fd, &buf[1], 1) == 1)&&
			(rv = read(fd, &buf[2], 1))){
		fill_glyph(glyph,buf,conversion,STDIN_FILENO);
		

		glyph = swap_endianness(glyph);
		

		
		write_glyph(glyph);

		
		memset(glyph, 0, sizeof(Glyph));
	        
	}
	*/
}


	
	endRead = times(&endReadT);
	realTime = (endRead-startRead);
	userTime = (endReadT.tms_utime-startReadT.tms_utime);
	sysTime = (endReadT.tms_stime-startReadT.tms_stime);


	endRead2 = times(&endReadT2);
	realTime2 = (endRead2-startRead2);
	userTime2 = (endReadT2.tms_utime-startReadT2.tms_utime);
	sysTime2 = (endReadT2.tms_stime-startReadT2.tms_stime);

	endRead1 = times(&endReadT1);
	realTime1 = (intmax_t)(endRead1-startRead1);
	userTime1 = (intmax_t)(endReadT1.tms_utime-startReadT1.tms_utime);
	sysTime1 = (intmax_t)(endReadT1.tms_stime-startReadT1.tms_stime);


	
	if(v2){
	printV2();
}
//printf("FILEOUT: %s\n", fileout);
free(filename);
//free(fileout);
free(glyph);
	quit_converter(NO_FD);
	return EXIT_SUCCESS;
}
}


Glyph* swap_endianness(Glyph* glyph) {
	

	if(source!=conversion){
	glyph->bytes[0] ^= glyph->bytes[1];
	glyph->bytes[1] ^= glyph->bytes[0];
	glyph->bytes[0] ^= glyph->bytes[1];
	
	if(glyph->surrogate){  /* If a surrogate pair, swap the next two bytes. */
		glyph->bytes[2] ^= glyph->bytes[3];
		glyph->bytes[3] ^= glyph->bytes[2];
		glyph->bytes[2] ^= glyph->bytes[3];
	}
}
	glyph->end = conversion;
	return glyph;
}


Glyph* fill_glyph(Glyph* glyph,unsigned int data[4], endianness end, int* fd) 
{

	if(source==LITTLE || source == BIG){
	glyph->bytes[0] = data[0];
	glyph->bytes[1] = data[1];
	
	unsigned int bits;
	bits= 0; 
	bits |= (data[1] + (data[0] <<8));
	glyph->surrogate = true;
	glyph->end = end;
	//glyphCount += 1;
	if(true){
		return glyph;
	}
	/* Check high surrogate pair using its special value range.*/
	
	
	if(bits > 55296 && bits < 56319){ 
		if(read(*fd, &data[0], 1) == 1 && 
			read(*fd, &data[1], 1) == 1){
			//printf("%s\n", "FOUND SUR3");
			bits |= (data[1] + (data[0] << 8));
			
			if(bits > 56320 && bits < 57343){ /* Check low surrogate pair.*/
				glyph->surrogate = true; 
			//	printf("%s\n", "FOUND SUR");
			} else {
			//	printf("%s\n", "FOUND SUR1");
				lseek(*fd, -OFFSET, SEEK_CUR); 
				glyph->surrogate = false;
			}
		}
	}
	if(!glyph->surrogate){
		glyph->bytes[3] = glyph->bytes[4] |= 0;
	} else {
		glyph->bytes[3] = data[1]; 
		glyph--->bytes[4] = data[2];
	}
	
	
	}

	else if ( source ==EIGHT &&utfCount == 0){
		//printf("%s\n", "Bomb is right");
		if(source == BIG){
			glyph->bytes[0] = 0xFE;
			glyph->bytes[1] = 0xFF;
		}
		if(source == LITTLE){
			glyph->bytes[0] = 0xFF;
			glyph->bytes[1] = 0xFE;
		}
	}
	else if( utfCount == 1){

		unsigned int bit1 = data[0]<<3>>3;
		//unsigned int bit2 = data[1];
		if(source == BIG){
			glyph ->bytes[0]= 0;
			glyph->bytes[1] = bit1;
 		}
		if(source ==LITTLE){
			glyph->bytes[0] = bit1;
			glyph->bytes[1]=0;
		}

	}
	else if(utfCount == 2){

	}

	return glyph;
}

/*
Glyph* convert(Glyph* glyph, endianness end){	
	//unsigned int bits;

	return NULL;
}
*/

void write_glyph(Glyph* glyph)
{
	if(!strcmp(fileout,"stdout")){
	if(glyph->surrogate){
		unsigned char* test = malloc(sizeof(char) * SURROGATE_SIZE);
		int x;
		for(x = 0 ; x < SURROGATE_SIZE; x++){
			test[x] = glyph->bytes[x];
			
		}
		//free(test);
		write(STDOUT_FILENO, test, SURROGATE_SIZE);
		
		free(test);
		glyphCount++;
	} else {
		unsigned char* test = malloc(sizeof(char) * NON_SURROGATE_SIZE);
		int x;
		for(x = 0 ; x < NON_SURROGATE_SIZE; x++){
			test[x] = glyph->bytes[x];
			
		}
		write(STDOUT_FILENO, test, NON_SURROGATE_SIZE);
		//free(test);
		
		free(test);
		glyphCount++;
	}

}
else{
	FILE *file = fopen(fileout, "ab+");
	int x;
	int surx;
	
	if(glyph->surrogate){
		surx = SURROGATE_SIZE;
	}
	else{
		surx = NON_SURROGATE_SIZE;
	}
	unsigned char* test = malloc(sizeof(char) * surx);
	for(x = 0 ; x < surx; x++){
			test[x] = glyph->bytes[x];
			
			fwrite(&test[x],sizeof(char),1,file);
		}
	

	free(test);
	fclose(file);
	glyphCount++;
		
		
}

}

void quit_converter(int fd)
{
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	if(fd != NO_FD)
		close(fd);
	exit(0);
	
}
void print_help() {
	int i;
	for(i = 0; i <11; i++){
		 
		printf("%s\n",USAGE[i] );
	}
	quit_converter(NO_FD);
}

void parse_args(int argc, char** argv)
{
	int option_index;
	option_index = 0;
	int c;
	c = 0;
	char* endian_convert ;
	endian_convert= NULL;

	static struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"UTF=", required_argument, 0, 'u'},
		{"h", no_argument, 0, 'h'},
		{"v", no_argument, 0, 'v'},
		{"vv", no_argument, 0, 't'},
		{"u", required_argument, 0, 'u'},
		{0, 0, 0, 0}
	};

	
	/* If getopt() returns with a valid (its working correctly) 
	 * return code, then process the args! */
	while ((c = getopt_long(argc, argv, "tvhu:", long_options, &option_index)) 
			!= -1) {
		
		switch(c){ 
				
				

				case 'h':
				
				print_help();
				quit_converter(NO_FD);
				
				break;

				
			case 'u':
				
				endian_convert = optarg;
				
				
				if(endian_convert == NULL){
				
					quit_converter(NO_FD);
					print_help();
					break;
				}
				

				if(!strcmp(endian_convert, "16LE")){ 
					conversion = LITTLE;
					
				} else if(!strcmp(endian_convert, "16BE")){
						conversion = BIG;
						
				} else {
					print_help();
					quit_converter(NO_FD);
					break;
				}

				

				break;
				case 'v':
				
				if(v1 == true && v2 == false){
					v2 = true;
					v1 = false;
				
				}else if(v1==false && v2 == true){
					v2 = true;
					v1 = false;
				}
				else{
					v1 = true;
				}
				
				break;


				case't':

				v2 = true;
				v1 = false;

				
				break;

			
			default:
				
				print_help();


				quit_converter(NO_FD);
				
		}

	}
	int x;
	for(x = 1; x <argc; x++){
		char firstChar = argv[x][0];
		if(!(firstChar==45)){
			
			if((filename==NULL)&&(strcmp(argv[x],"16LE")&&strcmp(argv[x],"16BE"))){
				filename = malloc(sizeof(char) * strlen(argv[x]));
					filename = strcpy(filename, argv[x]);
			}
			else if((fileout==NULL)&&(strcmp(argv[x],"16LE")&&strcmp(argv[x],"16BE"))){
				fileout = malloc(sizeof(char) * strlen(argv[x]));
					fileout = strcpy(fileout,argv[x]);
				fileout = argv[x];
			}
			else if((filename!=NULL)&&(fileout!=NULL)){
				break;
			}
		}
		
	}
	if(filename!= NULL && fileout==NULL){
		fileout = malloc(sizeof(char)*6);
		fileout = strcpy(fileout,"stdout");
	}

	if(v1){
		printV1();
	}
	
	if(filename==NULL){
		print_help();
		quit_converter(NO_FD);
	}
	if(optind==1){
		print_help();
	quit_converter(NO_FD);
	}
	
	

	
}


void printV1(){
	struct stat statistics;
	int x ;
	x = stat(filename,&statistics);
	
	if(x == -1){
		quit_converter(NO_FD);
	}

	
	int size;
	size = statistics.st_size;

	double size1 = (double)size/1000;
	
	char actpath[PATH_MAX+1];
	char *absPath = realpath(filename,actpath);

	
	char *fileEnc;

	if(source == 0){
		fileEnc = malloc(sizeof(char)*8);
		fileEnc = strcpy(fileEnc, "UTF-16LE");
	}
	if(source == 1){
		fileEnc = malloc(sizeof(char)*8);
		fileEnc = strcpy(fileEnc, "UTF-16BE");
	}
	if(conversion== 2){
		fileEnc = malloc(sizeof(char)*8);
		fileEnc = strcpy(fileEnc, "UTF-8");
	}


	
	char *outEnc;

	if(conversion == 0){
		outEnc = malloc(sizeof(char)*8);
		outEnc = strcpy(outEnc, "UTF-16LE");
	}
	if(conversion== 1){
		outEnc = malloc(sizeof(char)*8);
		outEnc = strcpy(outEnc, "UTF-16BE");
	}
	
	

	struct utsname hostMach;
	int name1 = uname(&hostMach);
	if(name1 ==-1){
		quit_converter(NO_FD);
	}

	char machName[256];
	int mac1 = gethostname(machName,sizeof(machName));

	if(mac1 == -1){
		quit_converter(NO_FD);
	}


	printf("input file size: %.02f kb\n", size1);
	printf("Input file path: %s\n", absPath);
	printf("Input file encoding: %s\n", fileEnc);
	printf("Output encoding: %s\n", outEnc);
	printf("Hostmachine: %s\n", machName);
	printf("Operating System: %s\n", hostMach.sysname);

	
	free(fileEnc);
	free(outEnc);


}

void printV2(){
	struct stat statistics;
	int x;
	x = stat(filename,&statistics);
	
	if(x == -1){
		quit_converter(NO_FD);
	}

	
	
	int size;
	size = statistics.st_size;

	double size1 = (double)size/1000;
	
	char actpath[PATH_MAX+1];
	char *absPath = realpath(filename,actpath);

	int countChar = 0;
	printf("%s\n",filename );
	FILE *file = fopen(filename,"r");
	while((char)fgetc(file)!= EOF){
		printf("%s\n", "in here");
		countChar++;

	}

	fclose(file);
	char *fileEnc;

	if(source == 0){
		fileEnc = malloc(sizeof(char)*8);
		fileEnc = strcpy(fileEnc, "UTF-16LE");
	}
	if(source == 1){
		fileEnc = malloc(sizeof(char)*8);
		fileEnc = strcpy(fileEnc, "UTF-16BE");
	}
	if(conversion== 2){
		fileEnc = malloc(sizeof(char)*8);
		fileEnc = strcpy(fileEnc, "UTF-8");
	}
	
	
	char *outEnc;

	if(conversion == 0){
		outEnc = malloc(sizeof(char)*8);
		outEnc = strcpy(outEnc, "UTF-16LE");
	}
	if(conversion == 1){
		outEnc = malloc(sizeof(char)*8);
		outEnc = strcpy(outEnc, "UTF-16BE");
	}



	struct utsname hostMach;
	int name1 = uname(&hostMach);
	if(name1 ==-1){
		quit_converter(NO_FD);
	}

	
	char machName[256];
	int mac1 = gethostname(machName,sizeof(machName));

	if(mac1 == -1){
		quit_converter(NO_FD);
	}
	printf("Input file size: %.02f kb\n", size1);
	printf("Input file path: %s\n", absPath);
	printf("Input file encoding: %s\n", fileEnc);
	printf("Output encoding: %s\n", outEnc);
	printf("Hostmachine: %s\n", machName);
	printf("Operating System: %s\n", hostMach.sysname);
	printf("Reading: real=%jd user=%jd  sys= %jd\n", (intmax_t)realTime,(intmax_t)userTime,(intmax_t)sysTime);
	printf("Converting: real=%jd user=%jd sys=%jd \n", (intmax_t)realTime1,(intmax_t)userTime1,(intmax_t)sysTime1);
	printf("Writing: real=%jd user=%jd sys=%jd \n", (intmax_t)realTime2,(intmax_t)userTime2,(intmax_t)sysTime2);
	printf("ASCII: %s\n", "");
	printf("Surrogates: %s\n", "");
	printf("Glyphs: %i\n", glyphCount);

	free(fileEnc);
	free(outEnc);
	
	
}
