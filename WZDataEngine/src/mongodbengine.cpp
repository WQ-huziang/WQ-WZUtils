//
//
// Author : huziang
// this is a cpp file complete mongodb interface in c++

#include "mongodbengine.h"
#include <bsoncxx/json.hpp>
#include <mongocxx/instance.hpp>
#include "todocuments.h"
#include "dataparse.h"

using mongocxx::cursor;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

MongodbEngine::MongodbEngine() {
  isWriting = 0;
  isReading = 0;
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
  if (isWriting || isReading)
    return -1;   // set write flag
  else
    isWriting = 1;

  // get document
  document doc;
  toDocument(md, doc);

  // get collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.insert_one(doc << finalize);

  // reset write flag
  isWriting = 0;
  
  return (bool)result;
}

int MongodbEngine::insert_many(const vector<map<string, string>> &mds) {
  if (isWriting || isReading)
    return -1;
  else 
    isWriting = 1;

  // get document
  vector<bsoncxx::document::value> docvs;
  toDocument(mds, docvs);

  // get collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.insert_many(docvs);

  // reset write flag
  isWriting = 0;

  return result->inserted_count();
}

int MongodbEngine::update_one(const KeyValue &filter, const vector<KeyValue> &update) {
  if (isWriting || isReading)
    return -1;
  else
    isWriting = 1;

  // get document
  document filterdoc {};
  document updatedoc {};
  toDocument(filter, update, filterdoc, updatedoc);

  // get collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.update_one(filterdoc << finalize, updatedoc << finalize);

  // reset write flag
  isWriting = 0;

  return (bool)result;
}

int MongodbEngine::update_many(const KeyValue &filter, const vector<KeyValue> &update) {
  if (isWriting || isReading)
    return -1;
  else
    isWriting = 1;

  // get document
  document filterdoc {};
  document updatedoc {};
  toDocument(filter, update, filterdoc, updatedoc);

  // get collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.update_many(filterdoc << finalize, updatedoc << finalize);

  // reset write flag
  isWriting = 0;

  return result->modified_count();
}

int MongodbEngine::find_one(map<string, string> &md, const vector<KeyValue> &condition, const char ID[20]) {
  if (isWriting)
    return -1;
  else 
    isReading++; // add reader

  // get document
  document doc {};
  toDocument(condition, ID, doc);

  // get one collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.find_one(doc << finalize);

  isReading--; // sub reader

  if (result) {
    string json = bsoncxx::to_json(*result);
    parseTo(md, json);
    map<string, string>::iterator it = md.begin();
    return 1;
  }
  return 0;
}

int MongodbEngine::find_many(vector<map<string, string>> &mds, const vector<KeyValue> &condition, const char ID[20]) {
  if (isWriting)
    return -1;
  else
    isReading++; // add reader

  // get document
  document doc {};
  toDocument(condition, ID, doc);
  std::cout << bsoncxx::to_json(doc) << std::endl;

  // get many collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  mongocxx::cursor cursor = coll.find(doc << finalize);

  isReading--; // sub reader

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
  if (isWriting || isReading)
    return -1;
  else 
    isWriting = 1;

  // get document
  document doc {};
  toDocument(condition, ID, doc);

  // get one collection
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];
  auto result = coll.delete_one(doc << finalize);

  // reset write flag
  isWriting = 0;

  return (bool)result;
}

int MongodbEngine::delete_many(const vector<KeyValue> &condition, const char ID[20]) {
  if (isWriting || isReading)
    return -1;
  else
    isWriting = 1;

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
  
  // reset write flag
  isWriting = 0;

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

int MongodbEngine::get_max_item(map<string, string> &md, const string &condition) {
  if (isWriting)
    return -1;
  else
    isReading++; // add reader

  // sort & limit
  document doc {};
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];

  mongocxx::options::find opts;
  opts.sort(make_document(kvp(condition, -1)));
  opts.limit(1);

  auto result = coll.find({}, opts);

  isReading--; // sub reader

  for (auto &&doc : result)
  {
    string json = bsoncxx::to_json(doc);
    parseTo(md, json);
    return 0;
  }
  return -1;
}

int MongodbEngine::get_latest_item(vector<map<string, string>> &mds, const char ID[20], int num){
  if (isWriting)
    return -1;
  else
    isReading++; // add reader

  document doc {};
  mongocxx::database db = conn.database(libname);
  mongocxx::collection coll = db[tablename];

  toDocument(ID, doc);

  mongocxx::options::find opts;
  opts.sort(make_document(kvp(TradingDay, -1), kvp(UpdateTime, -1)));
  opts.limit(num);

  auto cursor = coll.find(doc << finalize, opts);

  isReading--; // sub reader

  map<string, string> result;
  string json;
  for(auto &cur : cursor){
    json = bsoncxx::to_json(cur);
    parseTo(result, json);
    mds.push_back(result);
    result.clear();
  }
  return 0;
}