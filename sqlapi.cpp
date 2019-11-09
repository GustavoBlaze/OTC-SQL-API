#include "sqlapi.h"


int DatabaseSqlite::callback(void *p, int argc, char **argv, char **columnName){
  DatabaseSqlite* pthis = (DatabaseSqlite*)(p);

  for(int i = 0; i<argc; i++){
      pthis->onExec(std::string(columnName[i]), std::string(argv[i] ? argv[i] : "NULL"), i+1, argc);
  }
  return 0;
}

void DatabaseSqlite::onExec(std::vector<std::string> &columns, std::vector<std::string> &values){
  //callLuaField("onExec", columns, values);

  g_lua.pushObject(asLuaObject());
  g_lua.getField("onExec");
  if(!g_lua.isNil()){
    g_lua.insert(-2);
    g_lua.createTable(0, columns.size());
    for(int i = 0; i < columns.size(); i++){
      g_lua.pushString(values[i]);
      g_lua.setField(columns[i]);
    }
    g_lua.signalCall(1+1);
  }else{
    g_lua.pop(2);
  }
}

bool DatabaseSqlite::exec(const std::string& query){
  char *errorMsg = 0;
  int ret;

  ret = sqlite3_exec(database, query.c_str(), DatabaseSqlite::callback, (void*)this, &errorMsg);
  if (ret == SQLITE_OK){
    return true;
  }else{
    sqlite3_free(errorMsg);
    return false;
  }
}

bool DatabaseSqlite::open(const std::string& name){ // .db name file is the paramter
    int ret;
    ret = sqlite3_open(name.c_str(), &database);
    if (ret == SQLITE_OK){
        return true;
    }else{
        return false;
    }
}


void DatabaseSqlite::close(){
    sqlite3_close(database);
}
