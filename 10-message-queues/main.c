#include <stdio.h>
#include <string.h>
#include <mqueue.h>  
#include <errno.h>  
#include <stdlib.h>


#define MQ_MODE (S_IRUSR | S_IWUSR ) 

#define CLEAR_MSG_DATA(msg) \
    memset(&(msg), 0, sizeof(struct message))

struct message {
    unsigned int id;
    struct student {
        char name[50];
        int age;
        char country[100];
        char phone_number[100];
    } student;
};

/* Function to remove new line in string */
void rm_newLine(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

/* Function to split command */
int split_cmd(char *string, char *tokens[]) {
    int i = 0;
    char *token = strtok(string, " ");
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, " ");
    }
    return i;
}

void help_list() {   
    printf("\nAvailable Commands: \n");
    printf("help                                            - Display information about the available user interface options or command manual.\n");
    printf("input                                           - Input the student information.\n");
    printf("display <student id>                            - Display the information of student with id\n");
    printf("list                                            - Display a information list of all the students.\n");
    printf("delete <student id>                             - Delete the student information with student id from student list.\n");
    printf("exit                                            - Close program.\n");
    printf("****************************************************************\n\n");
}



struct message receive_mq_data(mqd_t mqid, struct mq_attr d_attr)
{
    struct message data;
    if(mq_receive(mqid, (char *)&data, d_attr.mq_msgsize, NULL) == -1) {  
            printf("mq_receive() dis error %d: %s\n", errno, strerror(errno));   
    }
    
    return data;
}

void send_mq_data(mqd_t mqid, struct message data)
{
    if(mq_send(mqid, (char *)&data, sizeof(data), 0) == -1) {  
        printf("mq_send() dis error %d: %s\n", errno, strerror(errno));  
    }
}

struct message rns_mq_data(mqd_t mqid, struct mq_attr d_attr)
{
    struct message data;
    if(mq_receive(mqid, (char *)&data, d_attr.mq_msgsize, NULL) == -1) {  
            printf("mq_receive() dis error %d: %s\n", errno, strerror(errno));   
    } else{
        if(mq_send(mqid, (char *)&data, sizeof(data), 0) == -1) {  
            printf("mq_send() dis error %d: %s\n", errno, strerror(errno));  
        }
    }
    
    return data;
}

void sort_mq_data(mqd_t mqid, unsigned int id, struct mq_attr d_attr)
{
    struct message data;
    unsigned int temp;
    for(int i = 0; i < (d_attr.mq_curmsgs - 1); i++){
        data = receive_mq_data(mqid, d_attr);
        if(data.id > id){
            temp = data.id;
            data.id = id;
            id = temp;
        }
        send_mq_data(mqid, data);
    }
}


void input_cmd(mqd_t mqid, unsigned int id) {
    struct message msg;
    msg.id = id;
    printf("Input student information ID: %u\n", msg.id);
    printf("Name: ");
    fgets(msg.student.name, sizeof(msg.student.name), stdin);
    rm_newLine(msg.student.name);
    printf("Age: ");
    scanf("%d", &(msg.student.age));
    getchar();
    printf("Country: ");
    fgets(msg.student.country, sizeof(msg.student.country), stdin);
    rm_newLine(msg.student.country);
    printf("Phone number: ");
    fgets(msg.student.phone_number, sizeof(msg.student.phone_number), stdin);
    rm_newLine(msg.student.phone_number);

    send_mq_data(mqid, msg);

}

void display_cmd(mqd_t mqid, unsigned int d_id, struct mq_attr d_attr)
{
    struct message data;
    long cur_prio = d_attr.mq_curmsgs;
    if(d_id >= cur_prio){
        printf("Unknown ID.....\n");
        return;
    }
    while(1){

        data = rns_mq_data(mqid, d_attr);
        if(data.id == d_id){
            printf("\nID: %d\t\tNAME: %s\t\tAGE: %d\t\tCOUNTRY: %s\t\tPhone number: %s\n\n"
                ,data.id, data.student.name, data.student.age, data.student.country, data.student.phone_number);
            return;
        }
    }
    
}

void list_cmd(mqd_t mqid, struct mq_attr d_attr)
{
    if(d_attr.mq_curmsgs < 1){
        printf("Not data to display.....\n");
        return;
    }
    struct message data;
    printf("\nThe total number of students: %ld\n", d_attr.mq_curmsgs);
    int i = 0;
    printf("\nID\t\tNAME\t\tAGE\t\tCOUNTRY\t\tPhone number\n");
    while(1){
        data = rns_mq_data(mqid, d_attr);
        if(data.id == i){
            printf("%d\t\t%s\t\t%d\t\t%s\t\t%s\n"
                ,data.id, data.student.name, data.student.age, data.student.country, data.student.phone_number);
            i++;
        }
        if(i >= d_attr.mq_curmsgs) return;
    }
}

void delete_cmd(mqd_t mqid, unsigned int d_id, struct mq_attr d_attr)
{
    struct message data;
    long cur_prio = d_attr.mq_curmsgs;
    if(d_id >= cur_prio){
        printf("Unknown ID.....\n");
        return;
    }
    while(1){
        data = receive_mq_data(mqid, d_attr);
        if(data.id == d_id){
            printf("Deleted information of student with ID %u...\n\n", data.id);
            sort_mq_data(mqid, d_id, d_attr);
            return;
        }
        send_mq_data(mqid, data);
    }
}

void invalid_cmd()
{
    printf("Invalid command. Type 'help' for list of available commands.\n");
}
  
int main(int argc, char *argv[]) {  

    struct mq_attr attr;


    mqd_t mqid = mq_open("/mqueue_student", O_RDWR | O_CREAT | O_NONBLOCK, 0666, NULL);  
    if (mqid == -1) {  
        printf("mq_open() error %d: %s\n", errno, strerror(errno));  
        return -1;  
    }

    unsigned int id;
    int spl_len;
    char cmd[100];
    char *spl_cmd[2];
    help_list();

    while (1) {
        if (mq_getattr(mqid, &attr) != 0) {  
            printf("mq_getattr() error %d: %s\n", errno, strerror(errno));  
            return -1;  
        }
        id = attr.mq_curmsgs;
        memset(&spl_cmd, 0, sizeof(spl_cmd));
        memset(&cmd, 0, sizeof(cmd));
        fgets(cmd, sizeof(cmd), stdin);
        rm_newLine(cmd);
        spl_len = split_cmd(cmd, spl_cmd);
        if(spl_len == 1){
            if(strcmp(spl_cmd[0], "help") == 0){
                help_list();
            } else if(strcmp(spl_cmd[0], "input") == 0){
                input_cmd(mqid, id);
            } else if(strcmp(spl_cmd[0], "list") == 0){
                list_cmd(mqid, attr);
            } else if(strcmp(spl_cmd[0], "exit") == 0){
                printf("Shutting down program....\n");
                break;
            } else{
                invalid_cmd();
            }
        } else if(spl_len == 2){
            if(strcmp(spl_cmd[0], "display") == 0){
                unsigned int d_id = (unsigned int) atoi(spl_cmd[1]);
                display_cmd(mqid, d_id, attr);
            } else if(strcmp(spl_cmd[0], "delete") == 0){
                unsigned int d_id = (unsigned int) atoi(spl_cmd[1]);
                delete_cmd(mqid, d_id, attr);
            } else{
                invalid_cmd();
            }
        } else{
            invalid_cmd();
        }

    }    
    mq_close(mqid);
    mq_unlink("/mqueue_student");
    return 0;  
}
