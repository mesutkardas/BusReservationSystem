#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>

struct trip{

    char trip_ID[10];
    char departure_city[40];
    char arrival_city[40];
    int departure_city_code;
    int arrival_city_code;
    char trip_date[20];
    char trip_time[10];
    char bus_licence_plate[25];
    char driver_name[30];
    int seat_count;
    int trip_price;

};

struct passenger{

    char name[15];
    char surname[15];
    long long int TR_ID_number;
    long long int phone_number;
    int seat_number;
    char wanted_ID[10];
    char card_number[30];
    char cardholder_name_surname[30];
    char expiration_date[20];
    char CVV[10];
};


void add_trip(struct trip *p_trip){

    printf("PLEASE ENTER THE DEPARTURE CITY: ");
    scanf("%s",p_trip->departure_city);
    getchar();

    printf("PLEASE ENTER THE ARRIVAL CITY: ");
    scanf("%s",p_trip->arrival_city);
    getchar();

    printf("PLEASE ENTER THE PLATE CODE OF THE DEPARTURE CITY: ");
    scanf("%d",&p_trip->departure_city_code);
    getchar();

    printf("PLEASE ENTER THE PLATE CODE OF THE ARRIVAL CITY: ");
    scanf("%d",&p_trip->arrival_city_code);
    getchar();

    printf("PLEASE ENTER THE TRIP DATE (dd/mm/year): ");
    scanf("%s",p_trip->trip_date);
    getchar();

    printf("PLEASE ENTER THE TRIP TIME (hour.minute): ");
    scanf("%s",p_trip->trip_time);
    getchar();

    printf("PLEASE ENTER THE BUS LICENCE PLATE(ex:34 AB 1234): ");
    fgets(p_trip->bus_licence_plate,sizeof(p_trip->bus_licence_plate),stdin);
    p_trip->bus_licence_plate[strcspn(p_trip->bus_licence_plate,"\n")]=0;

    printf("PLEASE ENTER THE DRIVER'S FULL NAME: ");
    fgets(p_trip->driver_name,sizeof(p_trip->driver_name),stdin);
    p_trip->driver_name[strcspn(p_trip->driver_name,"\n")]=0;

    printf("PLEASE ENTER THE COUNT OF SEATS OF THE BUS: ");
    scanf("%d",&p_trip->seat_count);
    getchar();

    printf("PLEASE ENTER THE TRIP PRICE: ");
    scanf("%d",&p_trip->trip_price);
    getchar();

    sprintf(p_trip->trip_ID,"%d%d%c%c",p_trip->departure_city_code,p_trip->arrival_city_code,
            p_trip->bus_licence_plate[3],p_trip->bus_licence_plate[4]);}


void writing_to_trip_file(FILE *file,struct trip p_trip){

    fprintf(file,"TRIP ID: %s\n",p_trip.trip_ID);
    fprintf(file,"TRIP PRICE: %d\n",p_trip.trip_price);
    fprintf(file,"DEPARTURE CITY: %s\n",p_trip.departure_city);
    fprintf(file,"ARRIVAL CITY: %s\n",p_trip.arrival_city);
    fprintf(file,"PLATE CODE OF THE DEPARTURE CITY: %d\n",p_trip.departure_city_code);
    fprintf(file,"PLATE CODE OF THE ARRIVAL CITY: %d\n",p_trip.arrival_city_code);
    fprintf(file,"TRIP DATE: %s\n",p_trip.trip_date);
    fprintf(file,"TRIP TIME: %s\n",p_trip.trip_time);
    fprintf(file,"BUS LICENCE PLATE: %s\n",p_trip.bus_licence_plate);
    fprintf(file,"DRIVER'S FULL NAME: %s\n",p_trip.driver_name);
    fprintf(file,"COUNT OF SEATS OF THE BUS: %d\n\n",p_trip.seat_count);
}

void last_file_operation(int number){

    if(number){

        remove("travel_information.txt");
        rename("temporary_file.txt", "travel_information.txt");
    }
    else{

        remove("temporary_file.txt");
    }
}

// CASE 1
void creating_a_new_bus_trip(int *trip_count){

    struct trip trips;
    FILE *created_trip;
    int size;
    
    printf("\nWELCOME TO CREATING A NEW BUS TRIP SCREEN.\n"
            "PLEASE ENTER THE REQUESTED VALUE BELOW.\n\n");

    add_trip(&trips);

    created_trip=fopen("travel_information.txt","a");
            
    if(created_trip==NULL){
        printf("THE FILE COULD NOT BE FOUND.\n");
    }

    else{

        fseek(created_trip,0,SEEK_END);
        size=ftell(created_trip);

        if ((size==0)){

            fprintf(created_trip,"----------------------------------------------------------------------\n");
            fprintf(created_trip,"%27s"," ");
            fprintf(created_trip,"ALL TRAVEL RECORDS\n");
            fprintf(created_trip,"----------------------------------------------------------------------\n\n");
        }
                
        fprintf(created_trip,"\n%d. TRIP RECORD:\n\n",*trip_count+1);

        writing_to_trip_file(created_trip,trips);
                
        fclose(created_trip);
    }

    printf("TRIP CREATING OPERATION COMPLETED SUCCESSFULLY.\n");
    printf("THE TRIP ID IS %s\n\n",trips.trip_ID);

    *trip_count+=1;

}

// CASE 2
void trip_update(){

    FILE *created_trip;
    FILE *temp_file;

    char line[200],wanted_ID[10],ID_in_file[10];
    struct trip temp_trip;
    int temp_trip_found=0;

    printf("\nWELCOME TO TRIP UPDATE SCREEN.\n");

    created_trip=fopen("travel_information.txt","r");
    temp_file=fopen("temporary_file.txt","w");

    if(created_trip==NULL || temp_file==NULL){

        printf("THE FILE COULD NOT FOUND.\n");
    }

    else{

        printf("PLEASE ENTER THE TRIP'S ID THAT YOU WANT TO UPDATE:");
        scanf("%s",wanted_ID);
        getchar();

        while(fgets(line,sizeof(line),created_trip)){

            if(strstr(line,"TRIP ID: ")){

                sscanf(line,"TRIP ID: %s",ID_in_file);

                if(strcmp(ID_in_file,wanted_ID)==0){
                            
                    temp_trip_found=1;

                    add_trip(&temp_trip);

                    writing_to_trip_file(temp_file,temp_trip);

                    for(int i=0;i<11;i++){
                        fgets(line,sizeof(line),created_trip);
                    }
                }
            }

            fputs(line,temp_file);
        }

        if(temp_trip_found==0){

            printf("\nTRIP ID %s NOT FOUND.\n",wanted_ID);
        }

        else{

            printf("\nUPDATE OPERATION COMPLETED SUCCESSFULLY.\n");
        }
    }

    fclose(created_trip);
    fclose(temp_file);

    last_file_operation(temp_trip_found);

}

// CASE 3
void trip_delete(){

    FILE *created_trip;
    FILE *temp_file;

    char line[200],wanted_ID[10],ID_in_file[10],empty_line[200],ID_line[200];
    int temp_trip_found=0,new_count=1;

    printf("\nWELCOME TO TRIP DELETE SCREEN.\n");

    created_trip=fopen("travel_information.txt","r");
    temp_file=fopen("temporary_file.txt","w");

    if(created_trip==NULL || temp_file==NULL){

        printf("THE FILE COULD NOT FOUND.\n");
    }

    else{

        printf("PLEASE ENTER THE TRIP'S ID THAT YOU WANT TO DELETE:");
        scanf("%s",wanted_ID);
        getchar();

        while(fgets(line,sizeof(line),created_trip)){

            if(strstr(line,"TRIP RECORD")){

                fgets(empty_line,sizeof(empty_line),created_trip);
                fgets(ID_line,sizeof(ID_line),created_trip);

                sscanf(ID_line,"TRIP ID: %s",ID_in_file);

                if(strcmp(ID_in_file,wanted_ID)==0){
                    temp_trip_found=1;

                    for(int i = 0; i <12; i++){
                        fgets(line, sizeof(line), created_trip);
                    }
                }

                else{

                    fprintf(temp_file,"%d. TRIP RECORD:\n",new_count);
                    fputs(empty_line, temp_file);
                    fputs(ID_line, temp_file);
                    new_count+=1;
                }
            }

            fputs(line,temp_file);
        }

        if(temp_trip_found==0){

            printf("\nTRIP ID %s NOT FOUND.\n",wanted_ID);
        }
        else{

            printf("\nDELETE OPERATION COMPLETED SUCCESSFULLY.\n");
        }
    }

    fclose(created_trip);
    fclose(temp_file);

    last_file_operation(temp_trip_found);

}

// CASE 4
void trip_inquiry(){

    FILE *created_trip;

    int temp_trip_found=0;
    char line[200],wanted_ID[10],ID_in_file[10];

    printf("\n WELCOME TO TRIP INQUIRY SCREEN.\n\n");

    created_trip=fopen("travel_information.txt","r");

    if(created_trip==NULL){
        printf("THE FILE COULD NOT BE FOUND");
    }

    else{
                
        printf("PLEASE ENTER THE TRIP'S ID THAT YOU WANT TO INQUIRY:");
        scanf("%s",wanted_ID);
        getchar();

        while(fgets(line,sizeof(line),created_trip)){

            if(strstr(line,"TRIP ID: ")){

                sscanf(line,"TRIP ID: %s",ID_in_file);

                if(strcmp(ID_in_file,wanted_ID)==0){

                    temp_trip_found=1;

                    printf("\n\nHERE ARE THE TRIP DETAILS:\n\n");

                        for(int j=0;j<11;j++){
                            fgets(line,sizeof(line),created_trip);
                            printf("%s",line);
                        }

                }
            }
        }

        if(temp_trip_found==0){
            printf("YOU ENTERED A WRONG ID\n");
        }
    }
    fclose(created_trip);
    printf("\nINQUIRY OPERATION COMPLETED SUCCESSFULLY.\n");

}

// CASE 5
void ticket_sell(int *passenger_count){

    FILE *created_trip;
    FILE *passenger_file;

    struct passenger passengers;
    int passenger_found=0,amount,total_seat_count,seat_number_in_p_file;
    long int size;
    char line[200],ID_in_file[10],TC_ID_line[200];

            
    created_trip=fopen("travel_information.txt","r");
    passenger_file=fopen("passenger_information.txt","a+");
            
    if(passenger_file==NULL || created_trip==NULL){
        printf("THE FILE COULD NOT BE FOUND.\n");
    }

    else{

        printf("\nWELCOME TO TICKET SELL SCREEN.\n"
                "PLEASE ENTER THE REQUESTED VALUE BELOW.\n\n");

        printf("PLEASE ENTER THE TRIP ID THAT YOU WANT TO BUY: ");
        scanf("%s",passengers.wanted_ID);
        getchar();

        while(fgets(line,sizeof(line),created_trip)){

            if(strstr(line,"TRIP ID: ")){

                sscanf(line,"TRIP ID: %s",ID_in_file);

                if(strcmp(ID_in_file,passengers.wanted_ID)==0){
                    passenger_found=1;
                }
            }

            if(strstr(line,"TRIP PRICE: ")){

                if(passenger_found==1){
                    sscanf(line,"TRIP PRICE: %d",&amount);
                }
            }

            if(strstr(line,"COUNT OF SEATS OF THE BUS: ")){

                if(passenger_found==1){
                    sscanf(line,"COUNT OF SEATS OF THE BUS: %d",&total_seat_count);
                    break;
                }
            }

                    
        }

        if(passenger_found==0){

            printf("THE ID THAT YOU ENTERED IS NOT FOUND.\n");
            fclose(created_trip);
            fclose(passenger_file);
        }

        else{

            printf("PLEASE ENTER YOUR NAME: ");
            fgets(passengers.name,sizeof(passengers.name),stdin);
            passengers.name[strcspn(passengers.name, "\n")] = 0;

            printf("PLEASE ENTER YOUR SURNAME: ");
            fgets(passengers.surname,sizeof(passengers.surname),stdin);
            passengers.surname[strcspn(passengers.surname, "\n")] = 0;

            printf("PLEASE ENTER YOUR TURKISH ID NUMBER: ");
            scanf("%lld",&passengers.TR_ID_number);
            getchar();

            printf("PLEASE ENTER YOUR PHONE NUMBER: ");
            scanf("%lld",&passengers.phone_number);
            getchar();

            while(true){
                    
                int is_seat_taken=0;
                char seat_line[200];

                printf("PLEASE ENTER THE SEAT NUMBER THAT YOU WANT (1-%d): ",total_seat_count);
                scanf("%d",&passengers.seat_number);
                getchar();

                if(passengers.seat_number>=total_seat_count || passengers.seat_number<1){

                    printf("INVALID SEAT NUMBER. THE ENTERED VALUE MUST BE BETWEEN 1-%d\n",total_seat_count);
                    continue;
                }
                        
                rewind(passenger_file);

                while(fgets(line,sizeof(line),passenger_file)){
                    
                    if(strstr(line,"PASSENGER'S TRIP ID: ")){

                        sscanf(line,"PASSENGER'S TRIP ID: %s",ID_in_file);

                        if(strcmp(passengers.wanted_ID,ID_in_file)==0){

                            fgets(TC_ID_line,sizeof(TC_ID_line),passenger_file);
                            fgets(seat_line,sizeof(seat_line),passenger_file);

                            sscanf(seat_line,"PASSENGER'S NUMBER OF SEAT: %d",&seat_number_in_p_file);

                            if(seat_number_in_p_file==passengers.seat_number){

                                printf("THIS SEAT IS ALREADY TAKEN. PLEASE CHOOSE ANOTHER ONE.\n");
                                is_seat_taken=1;
                                break;
                            }
                        }                
                    }
                }

                if(is_seat_taken==0){
                    break;
                }
            }
            printf("\nREDIRECTING YOU TO PAYMENT SCREEN...\n\n");

            printf("AMOUNT TO BE PAID: %d\n",amount);

            printf("PLEASE ENTER YOUR CREDIT CARD INFORMATION BELOW\n\n");

            printf("PLEASE ENTER YOUR CARD NUMBER(1234 5678 9012 3456):");
            fgets(passengers.card_number,sizeof(passengers.card_number),stdin);
            passengers.card_number[strcspn(passengers.card_number, "\n")] = 0;

            printf("PLEASE ENTER YOUR CARDHOLDER NAME AND SURNAME:");
            fgets(passengers.cardholder_name_surname,sizeof(passengers.cardholder_name_surname),stdin);
            passengers.cardholder_name_surname[strcspn(passengers.cardholder_name_surname, "\n")] = 0;
                
            printf("PLEASE ENTER YOUR EXPIRATION DATE(mm/yy):");
            scanf("%s",passengers.expiration_date);
            getchar();

            printf("PLEASE ENTER YOUR CVV(123):");
            scanf("%s",passengers.CVV);
            getchar();

            printf("\nYOUR PAYMENT IS BEING COMPLETED...\n");


            fseek(passenger_file,0,SEEK_END);
            size=ftell(passenger_file);

            if ((size==0)){

                fprintf(passenger_file,"----------------------------------------------------------------------\n");
                fprintf(passenger_file,"%20s"," ");
                fprintf(passenger_file,"ALL PASSENGER INFORMATION\n");
                fprintf(passenger_file,"----------------------------------------------------------------------\n\n");}
                
            fprintf(passenger_file,"\n%d. PASSENGER RECORD:\n\n",*passenger_count+1);

            fprintf(passenger_file,"PASSENGER'S TRIP ID: %s\n",passengers.wanted_ID);
            fprintf(passenger_file,"PASSENGER'S TURKISH ID NUMBER: %lld\n",passengers.TR_ID_number);
            fprintf(passenger_file,"PASSENGER'S NUMBER OF SEAT: %d\n",passengers.seat_number);
            fprintf(passenger_file,"PASSENGER'S NAME: %s\n",passengers.name);
            fprintf(passenger_file,"PASSENGER'S SURNAME: %s\n",passengers.surname);
            fprintf(passenger_file,"PASSENGER'S PHONE NUMBER: %lld\n",passengers.phone_number);

            fputs("\nPASSENGER'S CREDIT CARD INFORMATION:\n\n",passenger_file);
            fprintf(passenger_file,"PASSENGER'S CREDIT CARD NUMBER: %s\n",passengers.card_number);
            fprintf(passenger_file,"CARDHOLDER' NAME AND SURNAME: %s\n",passengers.cardholder_name_surname);
            fprintf(passenger_file,"CREDIT CARD'S EXPIRATION DATE: %s\n",passengers.expiration_date);
            fprintf(passenger_file,"CREDIT CARD'S CVV: %s\n",passengers.CVV);
                
            *passenger_count+=1;
        }      
    }

    fclose(passenger_file);
    fclose(created_trip);

    printf("\nTICKET SELL OPERATION COMPLETED SUCCESSFULLY.\n");;

}

// CASE 6
void ticket_cancel(){

    FILE *temp_file;
    FILE *passenger_file;

    int temp_trip_found=0,new_count=1;
    char line[200],wanted_ID[10],TC_ID_line[200],ID_line[200],empty_line[200],ID_in_file[10];
    long long int wanted_TC_ID,TC_ID_in_file;


    printf("\nWELCOME TO TICKET CANCELLATION SCREEN\n");

    passenger_file=fopen("passenger_information.txt","r");
    temp_file=fopen("temporary_file.txt","w");

    if(passenger_file==NULL || temp_file==NULL){

        printf("THE FILE COULD NOT FOUND.\n");
    }

    else{

        printf("PLEASE ENTER YOUR TURKISH ID NUMBER:");
        scanf("%lld",&wanted_TC_ID);
        getchar();

        printf("PLEASE ENTER THE TRIP'S ID THAT YOU WANT TO CANCEL:");
        scanf("%s",wanted_ID);
        getchar();

        while(fgets(line,sizeof(line),passenger_file)){

            if(strstr(line,"PASSENGER RECORD")){

                fgets(empty_line,sizeof(empty_line),passenger_file);
                fgets(ID_line,sizeof(ID_line),passenger_file);
                fgets(TC_ID_line,sizeof(TC_ID_line),passenger_file);

                sscanf(ID_line,"PASSENGER'S TRIP ID: %s",ID_in_file);
                sscanf(TC_ID_line,"PASSENGER'S TURKISH ID NUMBER: %lld",&TC_ID_in_file);

                if(strcmp(ID_in_file,wanted_ID)==0 && TC_ID_in_file==wanted_TC_ID){

                    temp_trip_found=1;

                    for(int i= 0; i<11; i++){

                        fgets(line,sizeof(line),passenger_file);
                    }
                }

                else{

                    fprintf(temp_file,"%d. PASSENGER RECORD:\n\n",new_count);

                    fputs(ID_line,temp_file);
                    fputs(TC_ID_line,temp_file);
                    new_count+=1;
                }
            }

            else{

                fputs(line,temp_file);
            }
        }
    }

    fclose(passenger_file);
    fclose(temp_file);

    if(temp_trip_found){

        remove("passenger_information.txt");
        rename("temporary_file.txt", "passenger_information.txt");
        printf("\nCANCEL OPERATION COMPLETED SUCCESSFULLY.\n");   
    }

    else{

        remove("temporary_file.txt");
        printf("\nYOU ENTERED INCORRECT VALUES.");
    }
}

// CASE 7
void create_ticket_receipt(){

    char name_in_file[20],surname_in_file[20],wanted_ID[10],line[200],ID_in_file[10],TC_ID_line[200];
    long long int phone_number_in_file,wanted_TC_ID,TC_ID_in_file;
    int temp_trip_found= 0,seat_number_in_p_file;

    FILE *ticket_receipt;
    FILE *passenger_file;


    passenger_file=fopen("passenger_information.txt","r");
    ticket_receipt=fopen("Ticket_Receipt.txt","w");

    if (passenger_file==NULL || ticket_receipt==NULL) {

        printf("THE FILE COULD NOT BE FOUND.\n");
    }

    else{

        printf("PLEASE ENTER YOUR TURKISH ID NUMBER: ");
        scanf("%lld",&wanted_TC_ID);
        getchar();

        printf("PLEASE ENTER THE TRIP'S ID: ");
        scanf("%s",wanted_ID);
        getchar();

        while(fgets(line,sizeof(line),passenger_file)) {

            if(strstr(line,"PASSENGER'S TRIP ID: ")) {

                sscanf(line,"PASSENGER'S TRIP ID: %s", ID_in_file);

                fgets(TC_ID_line,sizeof(TC_ID_line), passenger_file);
                sscanf(TC_ID_line,"PASSENGER'S TURKISH ID NUMBER: %lld",&TC_ID_in_file);

                if(strcmp(ID_in_file, wanted_ID)==0 && TC_ID_in_file==wanted_TC_ID) {
                        
                    temp_trip_found=1;

                    fprintf(ticket_receipt,"------------------------------------------------------\n");
                    fprintf(ticket_receipt,"           BLACKSEA TOURISM - E-TICKET RECEIPT        \n");
                    fprintf(ticket_receipt,"------------------------------------------------------\n\n");
                        
                    fprintf(ticket_receipt,"TRIP INFORMATION:\n\n");
                    fprintf(ticket_receipt,"TRIP ID: %s\n",ID_in_file);
                        
                    fgets(line,sizeof(line), passenger_file);
                    sscanf(line,"PASSENGER'S NUMBER OF SEAT: %d",&seat_number_in_p_file);
                    fprintf(ticket_receipt,"SEAT NUMBER: %d\n\n",seat_number_in_p_file);
                            
                    fprintf(ticket_receipt,"------------------------------------------------------\n\n");
                    fprintf(ticket_receipt,"PASSENGER DETAILS:\n\n"); 
                            
                    fgets(line,sizeof(line),passenger_file);
                    sscanf(line,"PASSENGER'S NAME: %s",name_in_file);
                    fprintf(ticket_receipt,"NAME: %s\n",name_in_file);
   
                    fgets(line,sizeof(line),passenger_file);
                    sscanf(line,"PASSENGER'S SURNAME: %s",surname_in_file);
                    fprintf(ticket_receipt,"SURNAME: %s\n",surname_in_file);

                    fprintf(ticket_receipt,"TC ID NUMBER: %lld\n",TC_ID_in_file);
 
                    fgets(line,sizeof(line),passenger_file);
                    sscanf(line,"PASSENGER'S PHONE NUMBER: %lld",&phone_number_in_file);
                    fprintf(ticket_receipt,"PHONE NUMBER: %lld\n\n\n",phone_number_in_file);

                    fprintf(ticket_receipt,"THANK YOU FOR CHOOSING US! HAVE A NICE TRIP.\n");

                    printf("\nTICKET RECEIPT CREATED SUCCESSFULLY.\n");
                    break; 
                }
            }
        }
    }

    if (temp_trip_found == 0) {
        printf("\nYOU ENTERED INCORRECT VALUE.\n");
    }

    fclose(passenger_file);
    fclose(ticket_receipt);

}

// CASE 8
void trip_list(){

    FILE *created_trip;

    char line[200];

    created_trip=fopen("travel_information.txt", "r");

    if (created_trip==NULL) {

        printf("THE TRAVEL INFORMATION FILE IS EMPTY OR NOT FOUND.\n");
    }

    else {

        while (fgets(line,sizeof(line),created_trip)) {
            printf("%s",line);
        }
                
        fclose(created_trip);
    }

    printf("\nLISTING OPERATION COMPLETED.\n");
}

// CASE 9
void app_close(){

    printf("\nCLOSING THE APPLICATION...\n\n");
    exit(0);
}

int main(){

    FILE *read_passenger_file;
    FILE *read_travel_file;

    int choice,trip_count=0,passenger_count=0;
    char line[200];
    

    // DETERMINING THE NUMBER OF TRIPS
    read_travel_file=fopen("travel_information.txt","r");

    if(read_travel_file!=NULL){

        while (fgets(line,sizeof(line),read_travel_file)){

            if(strstr(line,"TRIP RECORD")){
                trip_count+=1;
            }
        }
        fclose(read_travel_file);}

    else{
        printf("THE FILE COULD NOT FOUND. A NEW FILE WILL BE CREATED UPON FIRST ENTRY.\n");
    }

    // DETERMINING THE COUNT OF PASSENGERS
    read_passenger_file=fopen("passenger_information.txt","r");

    if(read_passenger_file!=NULL){

        while (fgets(line,sizeof(line),read_passenger_file)){

            if(strstr(line,"PASSENGER RECORD")){
                passenger_count+=1;
            }
        }
        fclose(read_passenger_file);
    }

    else{
        printf("THE FILE COULD NOT FOUND.\n");
    }

    // WELCOME SCREEN
    printf("\n----------------------------------------------------------------------\n");
    printf("%15s"," ");
    printf("WELCOME TO BLACKSEA TOURİSM APPLICATION\n");
    printf("----------------------------------------------------------------------\n");

    printf("OPTIONS:\n\n"
        "1.CREATING A NEW BUS TRIP\n"
        "2.TRIP UPDATE\n"
        "3.DELETE TRIP\n"
        "4.TRIP INQUIRY\n"
        "5.TICKET SELL\n"
        "6.TICKET CANCELLATION\n"
        "7.CREATING A TICKET RECEIPT\n"
        "8.ALL AVAILABLE BUS TRIP\n"
        "9.EXIT\n\n");
    
    // SELECTION POINT
    while (true){

        printf("PLEASE ENTER THE NUMBER OF THE OPERATION YOU WANT TO PERFORM:");

        if(scanf("%d",&choice)!=1){
            
            printf("\nINVALID NUMBER! PLEASE ENTER A NUMBER BETWEEN 1 AND 9.\n");
            while(getchar()!='\n'){
            }
            continue;
        }

        
        // PROCESS POINT
        switch (choice){

        case 1:

            creating_a_new_bus_trip(&trip_count);
            break;

        case 2:

            trip_update();
            break;
               
        case 3:

            trip_delete();
            break;

        case 4:

            trip_inquiry();
            break;

        case 5:

            ticket_sell(&passenger_count);
            break;
        
        case 6:

            ticket_cancel();
            break;
            
        case 7:

            create_ticket_receipt();
            break;
        
        case 8:
            
            trip_list();
            break;

        case 9:

            app_close();
            break;

        default:

            printf("INVALID NUMBER! PLEASE ENTER A NUMBER BETWEEN 1 AND 9.\n");
            break;
        }
    }

    return 0;
}