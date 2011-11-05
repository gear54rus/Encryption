#include "md5.h"

UnicodeString properSize(int size) {
	double size2 = size;
	int k = 0;
	for (; size2 > 1024; size2 = size2 / 1024, k++);
	size2 = double(int(size2 * 100)) / 100;
	switch (k) {
	case 0:
		return (UnicodeString(size2) + " Bytes");
	case 1:
		return (UnicodeString(size2) + " KiB");
	case 2:
		return (UnicodeString(size2) + " MiB");
	case 3:
		return (UnicodeString(size2) + " GiB");
	case 4:
		return (UnicodeString(size2) + " TiB");
	default:
		return (UnicodeString(size2) + " >TiB");
	}
}

char* fileN(char& in) {
	char* buff = new char[strlen(&in)];
	if (!strstr(&in, ".enc"))
		return &in;
	int pos = (int)(strstr(&in, ".enc") - &in);
	strncpy(buff, &in, pos);
	strcpy(&in, buff);
	*(&in + pos) = '\0';
	delete[]buff;
	return &in;
}

int md5alg(int buffsize, char* in, char* pwd, char* out, int mode) {
	MD5 md5;
	if (mode) {
		out = fileN(*out);
		FILE *input, *output;
		if (!(input = fopen(AnsiString(in).c_str(), "r+b")))
			return 1;
		if (!(output = fopen((AnsiString(out) + ".enc").c_str(), "w+b")))
			return 2;
		byte* buffer = new byte[buffsize];
		char* cipher;
		int hash, k = 0, marker = 0;
		char part[3];
		int readL = fread(buffer, 1, strlen(pwd) * 10, input);
		cipher = md5.digestMemory(buffer, readL);
		for (int i = 0; i < strlen(cipher); i += 2) {
			buffer[i / 2] =
				byte(strtol((strncpy(part, cipher + i, 2)), NULL, 16) % 256);
			marker++;
		}
		cipher = md5.digestString(pwd);
		k = strlen(strrchr(in, '\\') + 1);
		byte* namebuff = new byte[k];
		strcpy(namebuff, strrchr(in, '\\') + 1);
		for (int i = 0, j = 0; i < k;) {
			namebuff[i] =
				byte((strtol((strncpy(part, cipher + j, 2)), NULL,
				16) + namebuff[i]) % 256);
			marker++;
			j = j % 32;
			j += 2;
			i++;
		}
		marker += 4;
		fwrite(&marker, 4, 1, output);
		fwrite(buffer, strlen(cipher) / 2, 1, output);
		fwrite(namebuff, k, 1, output);
		fseek(input, 0, SEEK_SET);
		while (!feof(input)) {
			readL = fread(buffer, 1, buffsize, input);
			for (int i = 0, j = 0; i < readL; i++) {
				buffer[i] =
					byte((buffer[i] + strtol((strncpy(part, cipher + j, 2)),
					NULL, 16)) % 256);
				j = j % 32;
				j += 2;
				i++;
			}
			fwrite(buffer, 1, readL, output);
		}
		fclose(output);
		return 0;
	}

}
void BSum(byte1* arr,int n1,byte2* keyword,int n2,bool mode)//mode=1 - minus
{
	if (mode) mode=-1;
		else mode=1;
	for(int i=0,j=0;;i<n1;i++,j++)
	byte1[i]=(256+byte1[i]+mode*byte2[j%n2])%256;
}
