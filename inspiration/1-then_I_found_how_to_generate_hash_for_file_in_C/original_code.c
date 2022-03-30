void getFileHash(char *fileName){

unsigned char result[2*SHA_DIGEST_LENGTH];
unsigned char hash[SHA_DIGEST_LENGTH];
int i;
FILE *f = fopen(fileName,"rb");
SHA_CTX mdContent;
int bytes;
unsigned char data[1024];

if(f == NULL){
    printf("%s couldn't open file\n",fileName);
    exit(1);
}

SHA1_Init(&mdContent);
while((bytes = fread(data, 1, 1024, f)) != 0){

    SHA1_Update(&mdContent, data, bytes);
}

SHA1_Final(hash,&mdContent);

for(i=0;i<SHA_DIGEST_LENGTH;i++){
    printf("%02x",hash[i]);
}
printf("\n");
/** if you want to see the plain text of the hash */
for(i=0; i < SHA_DIGEST_LENGTH;i++){
    sprintf((char *)&(result[i*2]), "%02x",hash[i]);
}

printf("%s\n",result);

fclose(f);
}

Original code from: https://stackoverflow.com/questions/29416549/getting-hash-of-a-binary-file-c/29418717#29418717