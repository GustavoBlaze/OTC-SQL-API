//header
#ifndef __SQLAPI_H__
#define __SQLAPI_H__


#include <sqlite3.h>
#include "sqlitedeclaration.h"
#include <framework/luaengine/luaobject.h>
#include <framework/luaengine/luainterface.h>

class DatabaseSqlite : public LuaObject
{

public:
  static int callback(void *data, int argc, char **argv, char **columnName);
  bool exec(const std::string& query);
  bool open(const std::string& name);
  void onExec(std::string column, std::string value, int count, int total);
  void close();

protected:
  sqlite3 *database;
};

#endif
