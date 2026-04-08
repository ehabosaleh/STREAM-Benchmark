#include<cpuid.h>
#include<string.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__x86_64__) || defined(__i386__)
	#include<cpuid.h>
#endif

size_t detect_llc_size();
size_t detect_cache_cpuinfo();
size_t detect_cache_cpuid();

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
	printf("-------------------------------------------------------------\n");
	printf("Used /sys/devices/system/cpu/cpu0/cache/ to determine the cache size: %zu bytes\n",size);
	printf("-------------------------------------------------------------\n");
	return size;
}

size_t detect_cache_cpuinfo(){
	size_t size;
	FILE*f=fopen("/proc/cpuinfo","r");
  	if(!f){
        	printf("LLC detection failed. Using cpuid\n");
        	size=detect_cache_cpuid();
    	}

    	char line[256];
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
	printf("-------------------------------------------------------------\n");
	printf("Used cpuinfo to determine the cache size: %zu bytes\n",size);
	printf("-------------------------------------------------------------\n");
	return size;
	
}

size_t detect_cache_cpuid(){
	size_t size;
	unsigned int eax,ebx,ecx,edx;
	
	for(int i=0;;i++){
		__cpuid_count(4,i,eax,ebx,ecx,edx);
		int cache_type=eax&0x1f;
		if(cache_type==0) break;
		int level=(eax >> 5) & 0x7;
		if(level==3){
			int ways=((ebx >> 22) & 0x3FF) + 1;
			int partitions=((ebx >> 12) & 0x3FF) + 1;
			int line_size = (ebx & 0xFFF) + 1;
			int sets= ecx + 1;

            		size=(size_t)ways*partitions*line_size*sets;
		}
	}
	printf("-------------------------------------------------------------\n");
        printf("Used cpuid to determine the cache size: %zu bytes\n",size);
        printf("-------------------------------------------------------------\n");
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
