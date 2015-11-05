#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <sys/time.h>

int main (int argc, char *argv[])
{
        if (argc < 2){
                printf("a.out sql\n");
        }


        MYSQL conn;
        mysql_init(&conn);

        if (! mysql_real_connect(&conn, "192.168.49.189", "admin","admin","obtest", 2880, NULL, 0))
        {
                printf("Connection failed\n");
                return -1;
        }

        char error_info[1024] = {0};
        MYSQL_RES* result = 0;

        struct timeval t0,t1,t2;
        gettimeofday(&t0, 0);

        if (mysql_query(&conn, argv[1])){
                strncpy(error_info,mysql_error(&conn),1024);
                printf(error_info);
                mysql_close(&conn);
                return-1;
        }

        result = mysql_store_result(&conn);
        if (result == 0){
                strncpy(error_info,mysql_error(&conn),1024);
                printf(error_info);
                mysql_close(&conn);
                return -1;
        }

        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;
        gettimeofday(&t1, 0);

        while((row = mysql_fetch_row(result))) 
        {
                unsigned long *lengths = NULL;
                int i;
                for(i = 0; i < num_fields; i++)
                {
                        printf("%s\t",row[i]?row[i]:"NULL");
                }
                printf("\n");
        }
        gettimeofday(&t2, 0);

        mysql_free_result(result);
        mysql_close(&conn);

        long long diff = t1.tv_sec - t0.tv_sec;
        diff = diff * 1000000 + (t1.tv_usec - t0.tv_usec);
        printf("query takes time %llu\n", diff);
        diff = t2.tv_sec - t0.tv_sec;
        diff = diff * 1000000 + (t2.tv_usec - t0.tv_usec);

        printf("query and output take time %llu\n", diff);
        return 0;
}
