//
// cloriSearch core implementation
// version: 1.0 
// Copyright (C) 2018 James Wei (weijianlhp@163.com). All rights reserved
//
#include "internal/log.h"
#include "internal/json2pb.h"
#include "clorisearch.h"

namespace cloris {

CloriSearch::CloriSearch() {
}

CloriSearch::~CloriSearch() {
}

bool CloriSearch::Init(const std::string& source, IndexSchemaFormat format, SourceType source_type) {
    if (src_type != DIRECT) {
        cLog(ERROR, "cloriSearch init failed: unsupport src_type");
        return false;
    }
    if (format != ISF_JSON) {
        cLog(ERROR, "cloriSearch init failed: unsupport format-style schema now");
        return false;
    }
    IndexSchema schema;
    std::string err_msg;
    if (!json2pb::JsonToProtoMessage(source, &schema, &err_msg)) {
        cLog(ERROR, "cloriSearch init failed: json2pb error=" + err_msg);
        return false;
    }
    // init inverted index schema
    if (!inverted_index()->Init(schema, err_msg)) {
        cLog(ERROR, "cloriSearch init failed: inverted_index init failed");
        return false;
    }
    if (!forward_index()->Init()) {
        cLog(ERROR, "cloriSearch init failed: forward_index init failed");
        return false;
    }
    cLog(INFO, "cloriSearch init success!");
    return true;
}

bool CloriSearch::Load(const std::string& source, IndexSchemaFormat format, bool is_incremental) {
    if (format != ISF_JSON) {
        cLog(ERROR, "unsupport format-style");
        return false;
    }
    DNF dnf;
    std::string err_msg;
    if (!json2pb::JsonToProtoMessage(source, &dnf, &err_msg)) {
        cLog(ERROR, "CloriSearch load failed:" + err_msg);
        return false;
    }
    inverted_index()->Add(dnf);
}

std::vector<int> CloriSearch::Search(const Query& query, int limit) {
    return inverted_index()->Search(query, limit);
}

} // namespace cloris
