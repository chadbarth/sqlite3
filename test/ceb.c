#include "../sqlite3.h"
#include <stdio.h>

const char * create_statement = "CREATE TABLE IF NOT EXISTS \"production_metric\" (\"record_id\" INTEGER PRIMARY KEY ASC AUTOINCREMENT, \"calendar_day\" \"category_value\", \"calendar_day_event_id\" \"id\", \"calendar_day_number\" \"uint64\", \"duration\" \"real\", \"end_time\" \"date_time\", \"event_id\" \"id\", \"good_count\" \"double\", \"hour\" \"category_value\", \"hour_event_id\" \"id\", \"ideal_time\" \"real\", \"in_count\" \"double\", \"information_source\" \"category_value\", \"job\" \"category_value\", \"job_event_id\" \"id\", \"labor\" \"real\", \"modification_time\" \"date_time\", \"month\" \"category_value\", \"month_event_id\" \"id\", \"not_scheduled_time\" \"real\", \"pack_out_count\" \"double\", \"part\" \"category_value\", \"part_event_id\" \"id\", \"partial_cycle_time\" \"real\", \"partial_cycles\" \"uint64\", \"performance_impact\" \"category_value\", \"planned_stop_time\" \"real\", \"process_state\" \"category_value\", \"process_state_event_id\" \"id\", \"process_state_reason\" \"category_value\", \"production_day\" \"category_value\", \"production_day_event_id\" \"id\", \"production_day_number\" \"uint64\", \"production_phase\" \"category_value\", \"production_phase_event_id\" \"id\", \"quarter\" \"category_value\", \"quarter_event_id\" \"id\", \"record_order\" \"id\", \"record_version\" \"int64\", \"reject_count\" \"double\", \"run_time\" \"real\", \"shift\" \"category_value\", \"shift_event_id\" \"id\", \"shift_hour\" \"category_value\", \"shift_hour_event_id\" \"id\", \"slice_order\" \"id\", \"slow_cycle_lost_time\" \"real\", \"slow_cycle_time\" \"real\", \"slow_cycles\" \"uint64\", \"small_stop_lost_time\" \"real\", \"small_stop_time\" \"real\", \"small_stops\" \"uint64\", \"standard_cycle_lost_time\" \"real\", \"standard_cycle_time\" \"real\", \"standard_cycles\" \"uint64\", \"start_time\" \"date_time\", \"startup_rejects\" \"double\", \"suspect_cycle_lost_time\" \"real\", \"suspect_cycle_time\" \"real\", \"suspect_cycles\" \"uint64\", \"sync_id\" \"int64\", \"target_count\" \"double\", \"target_cycles\" \"uint64\", \"team\" \"category_value\", \"team_event_id\" \"id\", \"unplanned_stop_time\" \"real\", \"week\" \"category_value\", \"week_event_id\" \"id\", \"wip\" \"double\", \"year\" \"category_value\", \"year_event_id\" \"id\")";

const char * store_statement = "INSERT OR REPLACE INTO \"production_metric\" (\"calendar_day\",\"calendar_day_event_id\",\"calendar_day_number\",\"duration\",\"end_time\",\"event_id\",\"good_count\",\"hour\",\"hour_event_id\",\"ideal_time\",\"in_count\",\"information_source\",\"job\",\"job_event_id\",\"labor\",\"modification_time\",\"month\",\"month_event_id\",\"not_scheduled_time\",\"pack_out_count\",\"part\",\"part_event_id\",\"partial_cycle_time\",\"partial_cycles\",\"performance_impact\",\"planned_stop_time\",\"process_state\",\"process_state_event_id\",\"process_state_reason\",\"production_day\",\"production_day_event_id\",\"production_day_number\",\"production_phase\",\"production_phase_event_id\",\"quarter\",\"quarter_event_id\",\"record_id\",\"record_order\",\"record_version\",\"reject_count\",\"run_time\",\"shift\",\"shift_event_id\",\"shift_hour\",\"shift_hour_event_id\",\"slice_order\",\"slow_cycle_lost_time\",\"slow_cycle_time\",\"slow_cycles\",\"small_stop_lost_time\",\"small_stop_time\",\"small_stops\",\"standard_cycle_lost_time\",\"standard_cycle_time\",\"standard_cycles\",\"start_time\",\"startup_rejects\",\"suspect_cycle_lost_time\",\"suspect_cycle_time\",\"suspect_cycles\",\"sync_id\",\"target_count\",\"target_cycles\",\"team\",\"team_event_id\",\"unplanned_stop_time\",\"week\",\"week_event_id\",\"wip\",\"year\",\"year_event_id\") VALUES (:calendar_day,:calendar_day_event_id,:calendar_day_number,:duration,:end_time,:event_id,:good_count,:hour,:hour_event_id,:ideal_time,:in_count,:information_source,:job,:job_event_id,:labor,:modification_time,:month,:month_event_id,:not_scheduled_time,:pack_out_count,:part,:part_event_id,:partial_cycle_time,:partial_cycles,:performance_impact,:planned_stop_time,:process_state,:process_state_event_id,:process_state_reason,:production_day,:production_day_event_id,:production_day_number,:production_phase,:production_phase_event_id,:quarter,:quarter_event_id,:record_id,:record_order,:record_version,:reject_count,:run_time,:shift,:shift_event_id,:shift_hour,:shift_hour_event_id,:slice_order,:slow_cycle_lost_time,:slow_cycle_time,:slow_cycles,:small_stop_lost_time,:small_stop_time,:small_stops,:standard_cycle_lost_time,:standard_cycle_time,:standard_cycles,:start_time,:startup_rejects,:suspect_cycle_lost_time,:suspect_cycle_time,:suspect_cycles,:sync_id,:target_count,:target_cycles,:team,:team_event_id,:unplanned_stop_time,:week,:week_event_id,:wip,:year,:year_event_id)";

int main()
{
    sqlite3 *db;        // database connection
    int rc;             // return code
    char *errmsg;       // pointer to an error string

    /*
     * open SQLite database file test.db
     * use ":memory:" to use an in-memory database
     */
    rc = sqlite3_open("test_database", &db);
    if (rc != SQLITE_OK) {
        printf("ERROR opening SQLite DB: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    // Since we write quite often, we need to minimize the page size to reduce flash wear.
    // Default is 4096. https://sqlite.org/pragma.html#pragma_page_size
    sqlite3_exec(db, "PRAGMA page_size = 512", NULL, NULL, &errmsg);

    // Foreign key constraints are disabled by default (for backwards compatibility), so
    // must be enabled separately for each database connection.
    // http://www.sqlite.org/foreignkeys.html
    sqlite3_exec(db, "PRAGMA foreign_keys = ON", NULL, NULL, &errmsg);

    // We activate WAL mode (Write-Ahead Logging) so a hard power-down
    // won't corrupt the database file. Hopefully.
    // https://www.sqlite.org/wal.html
    sqlite3_exec(db, "PRAGMA journal_mode = WAL", NULL, NULL, &errmsg);

    rc = sqlite3_exec(db, create_statement, NULL, NULL, &errmsg);
    if (errmsg != NULL) {
        printf("Error in sqlite3_exec: %s\n", errmsg);
        sqlite3_free(errmsg);
    }

    sqlite3_exec(db, "CREATE INDEX \"production_metric_calendar_day_index\" on \"production_metric\" (\"calendar_day\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_calendar_day_event_id_index\" on \"production_metric\" (\"calendar_day_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_calendar_day_number_index\" on \"production_metric\" (\"calendar_day_number\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_event_id_index\" on \"production_metric\" (\"event_id\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_hour_index\" on \"production_metric\" (\"hour\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_hour_event_id_index\" on \"production_metric\" (\"hour_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_information_source_index\" on \"production_metric\" (\"information_source\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_job_index\" on \"production_metric\" (\"job\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_job_event_id_index\" on \"production_metric\" (\"job_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_month_index\" on \"production_metric\" (\"month\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_month_event_id_index\" on \"production_metric\" (\"month_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_part_index\" on \"production_metric\" (\"part\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_part_event_id_index\" on \"production_metric\" (\"part_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_performance_impact_index\" on \"production_metric\" (\"performance_impact\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_process_state_index\" on \"production_metric\" (\"process_state\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_process_state_event_id_index\" on \"production_metric\" (\"process_state_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_process_state_reason_index\" on \"production_metric\" (\"process_state_reason\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_production_day_index\" on \"production_metric\" (\"production_day\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_production_day_event_id_index\" on \"production_metric\" (\"production_day_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_production_day_number_index\" on \"production_metric\" (\"production_day_number\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_production_phase_index\" on \"production_metric\" (\"production_phase\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_production_phase_event_id_index\" on \"production_metric\" (\"production_phase_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_quarter_index\" on \"production_metric\" (\"quarter\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_quarter_event_id_index\" on \"production_metric\" (\"quarter_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_record_order_index\" on \"production_metric\" (\"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_shift_index\" on \"production_metric\" (\"shift\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_shift_event_id_index\" on \"production_metric\" (\"shift_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_shift_hour_index\" on \"production_metric\" (\"shift_hour\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_shift_hour_event_id_index\" on \"production_metric\" (\"shift_hour_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE UNIQUE INDEX \"production_metric_slice_order_index\" on \"production_metric\" (\"slice_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_sync_id_index\" on \"production_metric\" (\"sync_id\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_team_index\" on \"production_metric\" (\"team\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_team_event_id_index\" on \"production_metric\" (\"team_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_week_index\" on \"production_metric\" (\"week\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_week_event_id_index\" on \"production_metric\" (\"week_event_id\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_year_index\" on \"production_metric\" (\"year\", \"record_order\")", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE INDEX \"production_metric_year_event_id_index\" on \"production_metric\" (\"year_event_id\", \"record_order\")", NULL, NULL, NULL);

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, store_statement, -1, &stmt, NULL);

    rc = sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, ":calendar_day"), 1613422082216930395);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":calendar_day_event_id"), 2);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":calendar_day_number"), 17973);
    if(rc == SQLITE_OK) rc = sqlite3_bind_double(stmt, sqlite3_bind_parameter_index(stmt, ":duration"), 42.2);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, ":end_time"), 3180411448958606800);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":good_count"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":hour"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":hour_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":ideal_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":in_count"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":information_source"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":job"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":job_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":labor"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":modification_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":month"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":month_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":not_scheduled_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":pack_out_count"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":part"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":part_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":partial_cycle_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":partial_cycles"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":performance_impact"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":planned_stop_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":process_state"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":process_state_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":process_state_reason"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":production_day"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":production_day_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":production_day_number"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":production_phase"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":production_phase_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":quarter"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":quarter_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":record_id"), 9);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":record_order"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":record_version"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":reject_count"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":run_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":shift"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":shift_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":shift_hour"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":shift_hour_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":slice_order"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":slow_cycle_lost_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":slow_cycle_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":slow_cycles"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":small_stop_lost_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":small_stop_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":small_stops"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":standard_cycle_lost_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":standard_cycle_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":standard_cycles"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, ":start_time"), 3180411448958596800);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":startup_rejects"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":suspect_cycle_lost_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":suspect_cycle_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":suspect_cycles"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":sync_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":target_count"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":target_cycles"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":team"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":team_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":unplanned_stop_time"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":week"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":week_event_id"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":wip"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":year"), 12345);
    if(rc == SQLITE_OK) rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":year_event_id"), 12345);

    if (rc != SQLITE_OK) {
        printf("ERROR failed to bind: %s\n", sqlite3_errmsg(db));
        return -1;
    }


    printf("Executing statement\n");
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    printf("Statement complete\n");

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    printf("database closed.\n");
    return 0;
}
