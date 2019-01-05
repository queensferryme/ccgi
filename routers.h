/*
 * Route-View Functions
 * 
 * these functions each independently work as a module,
 * and handles HTTP requests from corresponding uri.
 */

#include <sqlite3.h>
#include <stdlib.h>

#include "utils.h"

#define CHUNKSIZE 1024
#define DB_PATH "students.db"
#define HTML_NL "<br>"

char temp[CHUNKSIZE];
sqlite3 *db;

void user(char *query_string) {
	// view function <user> => /user?id=<id:string>&name=<name:string>
	// @return: the matched record selected from database
	// initialize
	char id[20], name[20],
		 *sql = "SELECT * FROM students WHERE id=? AND name=?";
	sqlite3_stmt *stmt;
	parse_query_string(query_string, id, name);
	header("Content-Type", "application/json"); printf("\r\n");
	if(sqlite3_open(DB_PATH, &db) != SQLITE_OK || sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
		// fail to connect database or fail to prepare stmt statement
		printf("{%s}", json_int_value("count", -1, temp));
	else {
		// perform query operation
		sqlite3_bind_text(stmt, 1, id, strlen(id), NULL);
		sqlite3_bind_text(stmt, 2, name, strlen(name), NULL);
		if(sqlite3_step(stmt) == SQLITE_DONE) {
			printf("{%s}", json_int_value("count", 0, temp));
			return;
		}
		// output JSON data
		int i, columns = sqlite3_column_count(stmt);
		printf("{%s,\"result\":{", json_int_value("count", 1, temp));
		for(i = 0; i < columns; i++) {
			switch(sqlite3_column_type(stmt, i)) {
				case SQLITE_TEXT:
					printf("%s", json_str_value(sqlite3_column_name(stmt, i), sqlite3_column_text(stmt, i), temp));
					break;
				case SQLITE_INTEGER:
					printf("%s", json_int_value(sqlite3_column_name(stmt, i), sqlite3_column_int(stmt, i), temp));
					break;
			}
			printf("%c", i == columns - 1 ? '}' : ',');
		}
		printf("}");
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void admin() {
	// view function <admin> => /admin
	// @return: all record selected from database
	// initialize
	char *sql = "SELECT * from students";
	sqlite3_stmt *stmt;
	header("Content-Type", "application/json"); printf("\r\n");
	if(sqlite3_open(DB_PATH, &db) != SQLITE_OK || sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
		// fail to connect database or fail to prepare stmt statement
		printf("{%s}", json_int_value("count", -1, temp));
	else {
		// perform query operation & output JSON data
		int i, rows = 0, columns = sqlite3_column_count(stmt);
		printf("{\"result\":[{");
		while(sqlite3_step(stmt) == SQLITE_ROW) {
			if(rows++) printf(",{");
			for(i = 0; i < columns; i++) {
				switch(sqlite3_column_type(stmt, i)) {
					case SQLITE_TEXT:
						printf("%s", json_str_value(sqlite3_column_name(stmt, i), sqlite3_column_text(stmt, i), temp));
						break;
					case SQLITE_INTEGER:
						printf("%s", json_int_value(sqlite3_column_name(stmt, i), sqlite3_column_int(stmt, i), temp));
						break;
				}
				printf("%c", i == columns - 1 ? '}' : ',');
			}
		}
		printf("],%s}", json_int_value("count", rows, temp));
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void update(char *query_string) {
	// view function <upadte> => /update?id=<id:string>&name=<name:string>
	// @return: status of update operation
	// initailize
	char id[20], name[20], post_data[CHUNKSIZE / 2],
		 sql[CHUNKSIZE / 2] = "SELECT * FROM students WHERE id=? AND name=?";
	int math, physics, language,
		content_length = strtol(getenv("CONTENT_LENGTH"), NULL, 10);
	sqlite3_stmt *stmt, *update, *insert;
	header("Content-Type", "application/json"); printf("\r\n");
	// parsing post data & query string
	fgets(post_data, content_length + 1, stdin);
	parse_post_data(post_data, &math, &physics, &language);
	parse_query_string(query_string, id, name);
	if(sqlite3_open(DB_PATH, &db) != SQLITE_OK || sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
		// fail to connect database or fail to prepare stmt statement
		printf("{%s}", json_str_value("status", "failure", temp));
	else {
		// perform database operation
		sqlite3_bind_text(stmt, 1, id, strlen(id), NULL);
		sqlite3_bind_text(stmt, 2, name, strlen(name), NULL);
		if(sqlite3_step(stmt) == SQLITE_ROW) {
			// row existed, perform update operation
			strcpy(sql, "UPDATE students SET math=?, physics=?, language=? WHERE id=? AND name=?");
			sqlite3_prepare_v2(db, sql, -1, &update, NULL);
			sqlite3_bind_int(update, 1, math);
			sqlite3_bind_int(update, 2, physics);
			sqlite3_bind_int(update, 3, language);
			sqlite3_bind_text(update, 4, id, strlen(id), NULL);
			sqlite3_bind_text(update, 5, name, strlen(name), NULL);
			if(sqlite3_step(update) == SQLITE_DONE)
				printf("{%s}", json_str_value("status", "success", temp));
			else printf("{%s}", json_str_value("status", "failure", temp));
			sqlite3_finalize(update);
		}
		else {
			// row not existed, perform insert operation
			if(equal(id, "") || equal(name, "")) {
				printf("{%s}", json_str_value("status", "failure", temp));
				return;
			}
			strcpy(sql, "INSERT INTO students VALUES(?, ?, ?, ?, ?)");
			sqlite3_prepare_v2(db, sql, -1, &insert, NULL);
			sqlite3_bind_text(insert, 1, id, strlen(id), NULL);
			sqlite3_bind_text(insert, 2, name, strlen(name), NULL);
			sqlite3_bind_int(insert, 3, math);
			sqlite3_bind_int(insert, 4, physics);
			sqlite3_bind_int(insert, 5, language);
			sqlite3_step(insert);
			printf("{%s}", json_str_value("status", "success", temp));
			sqlite3_finalize(insert);
		}
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void delete(char *query_string) {
	// view function <delete> => /delete?id=<id:string>&name=<name:string>
	// @return: status of delete operation
	// initialize
	char id[20], name[20], *sql = "DELETE FROM students WHERE id=? AND name=?";
	sqlite3_stmt *stmt;
	header("Content-Type", "application/json"); printf("\r\n");
	if(sqlite3_open(DB_PATH, &db) != SQLITE_OK || sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
		// fail to connect database or fail to prepare stmt statement
		printf("{%s}", json_str_value("status", "failure", temp));
	else {
		// perform delete operation
		parse_query_string(query_string, id, name);
		sqlite3_bind_text(stmt, 1, id, strlen(id), NULL);
		sqlite3_bind_text(stmt, 2, name, strlen(name), NULL);
		sqlite3_step(stmt);
		printf("{%s}", json_str_value("status", "success", temp));
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void sysinfo() {
	// view function <sysinfo> => /sysinfo
	// @return: basic information of FastCGI environment
	header("Content-Type", "text/html"); printf("\r\n");
	printf("%s: %s%s", bold(temp, "QUERY_STRING"), getenv("QUERY_STRING"), HTML_NL);
	printf("%s: %s%s", bold(temp, "REQUEST_METHOD"), getenv("REQUEST_METHOD"), HTML_NL);
	printf("%s: %s%s", bold(temp, "CONTENT_TYPE"), getenv("CONTENT_TYPE"), HTML_NL);
	printf("%s: %s%s", bold(temp, "CONTENT_LENGTH"), getenv("CONTENT_LENGTH"), HTML_NL);
	printf("%s: %s%s", bold(temp, "SCRIPT_NAME"), getenv("SCRIPT_NAME"), HTML_NL);
	printf("%s: %s%s", bold(temp, "REQUEST_URI"), getenv("REQUEST_URI"), HTML_NL);
	printf("%s: %s%s", bold(temp, "DOCUMENT_URI"), getenv("DOCUMENT_URI"), HTML_NL);
	printf("%s: %s%s", bold(temp, "REMOTE_ADDR"), getenv("REMOTE_ADDR"), HTML_NL);
	printf("%s: %s%s", bold(temp, "REMOTE_PORT"), getenv("REMOTE_PORT"), HTML_NL);
}


