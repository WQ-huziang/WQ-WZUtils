//
//
// Author : huziang
// this is a cpp file complete mongodb interface in c++

#include "MongodbEngine.h"
#include <bsoncxx/json.hpp>
#include <mongocxx/instance.hpp>
#include "ToDocuments.h"
#include "DataParse.h"

using mongocxx::cursor;
using bsoncxx::builder::stream::finalize;

MongodbEngine::MongodbEngine() {
}

DataEngine* MongodbEngine::getInstance() {
  if (instance == NULL) {
    instance = new MongodbEngine();
  }
  return instance;
}

void MongodbEngine::init() {
  mongocxx::instance inst{};
  conn = mongocxx::client(mongocxx::uri("mongodb://localhost:27017"));
}

int MongodbEngine::insert_one(const map<string, string> &md) {
  // get document
  document doc;
  toDocument(md, doc);

  // get collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.insert_one(doc << finalize);
  return (bool)result;
}

int MongodbEngine::insert_many(const vector<map<string, string>> &mds) {
  // get document
  vector<bsoncxx::document::value> docvs;
  toDocument(mds, docvs);

  // get collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.insert_many(docvs);
  return result->inserted_count();
}

int MongodbEngine::update_one(const KeyValue &filter, const vector<KeyValue> &update) {
  // get document
  document filterdoc {};
  document updatedoc {};
  toDocument(filter, update, filterdoc, updatedoc);

  // get collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.update_one(filterdoc << finalize, updatedoc << finalize);
  return (bool)result;
}

int MongodbEngine::update_many(const KeyValue &filter, const vector<KeyValue> &update) {
  // get document
  document filterdoc {};
  document updatedoc {};
  toDocument(filter, update, filterdoc, updatedoc);

  // get collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.update_many(filterdoc << finalize, updatedoc << finalize);
  return result->modified_count();
}

int MongodbEngine::find_one(map<string, string> &md, const vector<KeyValue> &condition, const char ID[20]) {
  // get document
  document doc {};
  toDocument(condition, ID, doc);

  // get one collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.find_one(doc << finalize);
  if (result) {
    string json = bsoncxx::to_json(*result);
    parseTo(md, json);
    map<string, string>::iterator it = md.begin();
    return 1;
  }
  return 0;
}

int MongodbEngine::find_many(vector<map<string, string>> &mds, const vector<KeyValue> &condition, const char ID[20]) {
  // get document
  document doc {};
  toDocument(condition, ID, doc);
  std::cout << bsoncxx::to_json(doc) << std::endl;

  // get many collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  mongocxx::cursor cursor = coll.find(doc << finalize);

  // go through all answer
  int num = 0;
  map<string, string> result;
  string json;
  for(auto &cur : cursor){
    json = bsoncxx::to_json(cur);
    parseTo(result, json);
    mds.push_back(result);
    result.clear();
    num++;
  }

  return num;
}

int MongodbEngine::delete_one(const vector<KeyValue> &condition, const char ID[20]) {
  // get document
  document doc {};
  toDocument(condition, ID, doc);

  // get one collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.delete_one(doc << finalize);
  return (bool)result;
}

int MongodbEngine::delete_many(const vector<KeyValue> &condition, const char ID[20]) {
  // get document
  document doc {};
  toDocument(condition, ID, doc);

  // get many collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.delete_many(doc << finalize);
  if (result) {
    return result->deleted_count();
  }
  return 0;
}

int MongodbEngine::set_index(string index, bool isascending) {
  // get document
  document doc{};
  auto index_specification = doc << index << int(isascending ? 1 : -1) << finalize;

  // get one collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];

  // create index
  coll.create_index(std::move(index_specification));

  return 0;
}