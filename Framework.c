// CURD using Frameworks

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 60
#define DATAFILE "Framework.dat"
#define MENUFILE "Menu.cfg"
#define FIELDS_FILE "FieldNames.cfg"
#define CUSTOMIZED_FILE "CustomizedMessages.cfg"

FILE *fpDatafile;
FILE *fpFields;
FILE *fpCustomizedMessages;
char **fieldNames;
char **messageArray;
char ID[LENGTH];
char fieldData[LENGTH];
int fieldsCount, counter = 0;
char status;
int recordPosition;
char field[LENGTH];
int customMessageCount;

void printNotFoundMessage()
{
	printf("Hosteller details not found\n");
}


int getFieldsCount()
{
	fpFields = fopen(FIELDS_FILE, "r");
    fieldsCount = 0;
    while (fgets(field, LENGTH, fpFields))
    {
        fieldsCount++;
    }
    fclose(fpFields);
    return fieldsCount;
}

void getFieldNames()
{

	fieldNames;
	fieldsCount = getFieldsCount();
	fieldNames = malloc(sizeof(char*) * fieldsCount);
	fpFields = fopen(FIELDS_FILE, "r");
	char fieldName[LENGTH];
	counter = 0 ;
	while(fgets(fieldName, LENGTH, fpFields))
	{
		fieldNames[counter] =  malloc(sizeof(char) * LENGTH);
		fieldName[strlen(fieldName)-1] = '\0';
		strcpy(fieldNames[counter], fieldName);
		counter++;
	}
	

}

int getMessagesCount()
{
	char messages[LENGTH];
	customMessageCount = 0;
    fpCustomizedMessages = fopen(CUSTOMIZED_FILE, "r");
    while(fgets(messages, LENGTH, fpCustomizedMessages))
    {
   	    customMessageCount++;
    }
    fclose(fpCustomizedMessages);
    return customMessageCount;
}

void getCustomizedMessages()
{
	int counter = 0;
	char array[LENGTH];
	customMessageCount = getMessagesCount();
	messageArray = (char**) malloc(sizeof(char*) * customMessageCount);
	fpCustomizedMessages = fopen(CUSTOMIZED_FILE, "r");

	while(fgets(array, LENGTH, fpCustomizedMessages))
	{
		messageArray[counter]= (char*) malloc(sizeof(char) * LENGTH);
		array[strlen(array) - 1] = '\0';
		strcpy(messageArray[counter], array);
		counter++;
	}	   
	fclose(fpCustomizedMessages);
	
}	

void getRecordId()
{
	printf("\nEnter ID:");
	scanf(" %s", ID);
}

int getRecordPosition()
{
	getRecordId();
	
	int length, found = 0; 
	int dataFileSize;
	fpDatafile = fopen(DATAFILE, "r");
	fseek(fpDatafile, 0, SEEK_END);
	dataFileSize = ftell(fpDatafile);
	
	rewind(fpDatafile);

	getFieldsCount();
	getFieldNames();
	while(ftell(fpDatafile) < dataFileSize)
	{
		
		
		status = fgetc(fpDatafile);
		rewind(fpFields);
		if(status == 'A')
		{
			fread(fieldData, sizeof(char), LENGTH, fpDatafile);
		    
			if(strcmp(fieldData, ID) == 0)
			{
				return ftell(fpDatafile) - LENGTH;
			}
			else
			{
				fseek(fpDatafile, (fieldsCount - 1) * LENGTH, SEEK_CUR);
			}
		}
		else
		{
			fseek(fpDatafile, fieldsCount * LENGTH, SEEK_CUR);
		}
	}
	return -1;
}
void create()
{
	
	getFieldNames();
	fpDatafile = fopen(DATAFILE, "a");
	fputc('A', fpDatafile);
	fclose(fpDatafile);
	fpDatafile = fopen(DATAFILE, "a");
	for( counter =0 ; counter< fieldsCount; counter++)
	{
		printf("Enter %s: ", fieldNames[counter]);
		scanf(" %[^\n]s", &fieldData);
		fwrite(fieldData, sizeof(char), LENGTH, fpDatafile);
	}
	fclose(fpDatafile);
	// printf("%s",);

}
void search()
{
	
	recordPosition = getRecordPosition();
	int counter = 0;
	if(recordPosition != -1)
	{
		fpDatafile = fopen(DATAFILE, "r");
		fseek(fpDatafile, recordPosition, SEEK_SET);
		getFieldsCount();
		getFieldNames();
		printf("\n--------------------------------------------\n");
		while(counter< fieldsCount)
		{
			char fieldData[LENGTH];
			if (fread(fieldData, sizeof(char), LENGTH, fpDatafile))
			{
				printf("%s : %s\n",fieldNames[counter], fieldData );
				counter++;
			}
		}
		printf("\n--------------------------------------------\n");
	}
	else
	{
		printf("\n %s\n", messageArray[3]);
	}
	fclose(fpDatafile);
}

void showAll()
{
	printf("Show all\n");
	fpDatafile = fopen(DATAFILE, "r");
	int dataFileSize;
	
	fseek(fpDatafile, 0 ,SEEK_END);
	dataFileSize = ftell(fpDatafile);
	rewind(fpDatafile);
	getFieldsCount();
	getFieldNames();
	printf("\n---------------------------------\n");
	while(ftell(fpDatafile) < dataFileSize)
	{
		char status;
		rewind(fpFields);
		counter = 0;
		status = fgetc(fpDatafile);
		if (status == 'A')
		{
			while (counter < fieldsCount)
			{
				if (fread(fieldData, sizeof(char), LENGTH, fpDatafile))
				{
					printf("%s : %s\n", fieldNames[counter], fieldData);
					counter++;
				}
			}
			printf("\n------------------------------\n");
		}
	}
	fclose(fpDatafile);

}
void delete_()
{
	printf("delete\n");
	recordPosition = getRecordPosition();
	
	if (recordPosition != -1)
	{
		fpDatafile = fopen(DATAFILE, "r+");
		fseek(fpDatafile, recordPosition - 1, SEEK_SET);
		fputc('D', fpDatafile);
		fclose(fpDatafile);
	}
	else
	{
		printf("\nRecord not found\n");
	}
}
void update()
{
	printf("update\n");
	recordPosition = getRecordPosition();

	int choice;
	if(recordPosition != -1)
	{
		int counter = 1;
		getFieldNames();
		while(counter < fieldsCount)
		{
			printf("%d. %s\n", counter, fieldNames[counter]);
			counter++;
		}
		printf("\nEnter field to update:");
		scanf("%d", &choice);
		if(choice > 0 && choice < fieldsCount)
		{
			fpDatafile = fopen(DATAFILE, "r+");

			printf("Enter %s to update: ",fieldNames[choice]);
			scanf(" %[^\n]s", fieldData);
			fseek(fpDatafile, recordPosition + (LENGTH * choice),SEEK_SET);
			if(fwrite(fieldData, sizeof(char), LENGTH, fpDatafile))
			{
				printf("\n records updated...\n");
			}
			fclose(fpDatafile);
			fclose(fpFields);
		}
	}
	else
	{
		printf("Record not found\n");
	}
}

void menu()
{
	int choice;
	char searchId[LENGTH];
	FILE *fpMenu;
	fpMenu = fopen( MENUFILE, "r" );
	fseek( fpMenu, 0, SEEK_END );
	int menuSize = ftell( fpMenu ) + 1;
	fseek( fpMenu, 0 , SEEK_SET);
	char menu[menuSize];
	fread( menu, sizeof(char), menuSize - 1, fpMenu );
	fclose(fpMenu);

	menu[menuSize] = '\0';


    printf("\n%s\n", menu);

    printf("Enter your choice: ");
    scanf("%d", &choice);

    fieldsCount = getFieldsCount();
    getCustomizedMessages();

    switch (choice)
    {
            case 1 :  
                create();
                break;


            case 2 :  
                showAll();
                break;


            case 3 : 
            	search();
                break;


            case 4 : 
                update();
                break;


            case 5 :        
                delete_();
                break;
	    
            case 0 :
                printf("\nExited!\n");
                fclose(fpFields);
                exit(0);
                break;
	    
            default: 
                printf("\nInvalid input, Try again!\n");
                break;
        }
}
void main ()
{
	while(1)
	{
		menu();
	}
}
