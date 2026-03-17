#include<cpuid.h>
#include<string.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>

size_t detect_llc_size(){
	size_t size;
	FILE*f=fopen("/sys/devices/system/cpu/cpu0/cache/index3/size", "r");
    	if(!f){
        	printf("LLC detection failed. Using system file cpuinfo\n");
        	size=detect_cache_cpuinfo();
    	}
	char unit;
    	fscanf(f,"%zu%c",&size,&unit);
    	if(unit=='K')
        	size=size*1024;
    	else if(unit=='M')
        	size=size*1024*1024;
	fclose(f);
	return size;
}

size_t detect_cache_cpuinfo(){
	FILE*f=fopen("/proc/cpuinfo","r");
  	if(!f){
        	printf("LLC detection failed. Using default 32MB\n");
        	return 32*1024*1024;
    	}

    	char line[256];
    	size_t size;
	while(fgets(line,sizeof(line),f)){
        	if(strstr(line,"cache size")) {
            	char unit[2];
            	sscanf(line,"cache size : %zu %s",&size,unit);
            	if(strcmp(unit,"KB")==0)
                	size=size*1024;
            	else if(strcmp(unit,"MB")==0)
                	size=size*1024*1024;
    		}
	}
	fclose(f);
	return size;
	
}
void usage(const char *argv0) {
      fprintf(stderr,
          "Usage: %s [--array-size=N] [--mem-bound=N] [--iters=N]\n"
          "Examples:\n"
          "  %s --array-size=2000 \n"
          "  %s--llc-scale=n \n",
          argv0, argv0, argv0);
      exit(EXIT_FAILURE);
  }
