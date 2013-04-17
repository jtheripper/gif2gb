/*
 * gif2gb.c
 * 
 * Versão 0.2a - 17/04/2013_18:04
 * 
 * Copyright 2013 Jordy <jordy@jordy-Desktop>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define INSTRUCOES "\nUsage: gif2gb [options] *image*.gif *output*.c\n\n Version 0.2b\n\n"

FILE *Fp,*Fc;


void imgInTxt(char *filename)
{
  char s2[]=" -crop 8x8 1.txt";
	char s1[]="convert ";
	char s4[strlen(filename)+strlen(s1)+strlen(s2)+5];
	strcpy(s4,s1);
	strcat(s4,filename);
	strcat(s4,s2);
	system(s4);
	system("sed \"/# ImageMagick/d\" 1.txt > 2.txt");	
	system("sed \"s/.*://\" 2.txt > 1.txt");
	system("sed \"s/srgb.*//\" 1.txt > 2.txt");
	system("sed \"s/(.*)//\" 2.txt > 1.txt");
	system("sed \"s/white//\" 1.txt > 2.txt");
	system("sed \"s/black//\" 2.txt > 1.txt");
	system("sed \"s/   //\" 1.txt > 2.txt");
	system("sed \"s/  /;/\" 2.txt > pronto.txt");
	system("mv pronto.txt 2.txt");
	system("sed \"s/#//\" 2.txt > pronto.txt");
	system("rm 2.txt");
}

void fechar(int x)
{
	fclose(Fp);
	fclose(Fc);
	exit(x);
}

void abrirArq(char *s)
{
		if((Fp=fopen(s,"r+"))==NULL)
		{
				printf("\n\n*ALGO DEU ERRADO, FECHANDO PROGRAMA*\n\n");
				fechar(1);
		}
}

void criarArq(char *s)
{
		if((Fc=fopen(s,"w+"))==NULL)
		{
				printf("\n\n*ALGO DEU ERRADO, FECHANDO PROGRAMA*\n\n");
				fechar(2);
		}
}

void forHex(int numero, int i,  int *a,  int *b)
{
	switch(numero)
	{
		case 0xFFFFFF:
			break;
		case 0xAAAAAA:
			*b |= (int) pow(2,i);
			break;
		case 0x555555:
			*a |= (int) pow(2,i);
			break;
		case 0x000000:
			*a |= (int) pow(2,i);
			*b |= (int) pow(2,i);
			break;
		default:
			printf("\nNúmero Inválido, saindo!\n");
			fechar(3);
	}
	
	
}

int tamanhoArq()
{
	unsigned long sz;
	fseek(Fp, 0, SEEK_END); //procura o fim do arquivo.
	sz = ftell(Fp); //Diz o tamanho do arquivo depois que chegou ao fim.
	fseek(Fp,0,0);//O programa procura a PRIMEIRA letra do arquivo.
			
	return sz;
	
}

void escreverH()
{
	fprintf(Fc,"/* Arquivo criado pelo programa do Jordy */\n\n\n\nunsigned char Sprites[] = \n{ \n");	
}

void filtarArq()
{
		int x,y=0,a,b,Z;
		while(fscanf(Fp,"%X;",&Z)!=EOF)
		{
			a=b=0;
			x=0;
			forHex(Z,x,&a,&b);
			for(x=1;x<8;x++)
			{
				fscanf(Fp,"%X;",&Z);
				forHex(Z,x,&a,&b);
			}
			if(a==0)
				fprintf(Fc,"0x0%X,",a);
			else
				fprintf(Fc,"0x%X,",a);
			if(b==0)
				fprintf(Fc,"0x0%X,",b);
			else
				fprintf(Fc,"0x%X,",b);
			y++;
			if(y==4)
			{
				fprintf(Fc,"\n");
				y=0;
			}
		}
		system("rm pronto.txt");
		
}

void mostrarInstrucoes()
{
	printf(INSTRUCOES);
}

int main(int argc, char **argv)
{
	if(argv[1]==NULL || argv[2]==NULL || strcmp(argv[1],".gif")==0 || strcmp(argv[2],".c")==0)
	{
		mostrarInstrucoes();
		return 1;
	}
	else
		imgInTxt(argv[1]);
	abrirArq("pronto.txt");
	criarArq(argv[2]);
	escreverH();
	filtarArq();
	fprintf(Fc,"};");
	fechar(0);
	return 0;
}

