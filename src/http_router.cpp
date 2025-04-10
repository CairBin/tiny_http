#include "tiny_http/http_router.h"
#include <regex>

namespace tiny_http {

void RouterTireTree::InsertNode(Node* parent, const std::string& path, int start, int end, HttpHandler handler){
    if(start >= end){
        parent->handler_ = handler;
        return;
    }

    int next_slash = path.find('/', start);
    if(next_slash == std::string::npos || next_slash > end)
        next_slash = end;
    
    std::string sub_path = path.substr(start, next_slash - start);
    char first_char = sub_path[0];
    RouterNodeType node_type = RouterNodeType::STATIC;
    if(first_char == ':'){
        node_type = RouterNodeType::PARAM;
    }else if(first_char == '*'){
        node_type = RouterNodeType::CATCH_ALL;
    }

    if(parent->children_.find(first_char) == parent->children_.end()){
        if (parent->children_.find(first_char) == parent->children_.end()){
            Node *new_node = new Node();
            new_node->path_ = sub_path;
            new_node->wild_child_ = false;
            new_node->n_type_ = node_type;
            new_node->indices_ = "";
            new_node->handler_ = nullptr;
            parent->children_[first_char] = new_node;
            parent->indices_ += first_char;
            if (node_type == RouterNodeType::PARAM || node_type == RouterNodeType::CATCH_ALL)
                parent->wild_child_ = true;
            if (node_type == RouterNodeType::PARAM)
                new_node->param_names_.push_back(sub_path.substr(1));
        }
    }
    InsertNode(parent->children_[first_char], path, next_slash + 1, end, handler);
}

RouterTireTree::Node* RouterTireTree::FindNode(Node *parent, const std::string& path, int start, int end, std::unordered_map<std::string, std::string>& param_values){
    if(start >= end){
        return parent->handler_ ? parent : nullptr;
    }

    int next_slash = path.find('/', start);
    if(next_slash == std::string::npos || next_slash > end){
        next_slash = end;
    }

    std::string sub_path = path.substr(start, next_slash - start);
    char first_char = sub_path[0];

    if (parent->wild_child_){
        for (char index : parent->indices_){
            Node *child = parent->children_[index];
            if ((child->n_type_ == RouterNodeType::PARAM && sub_path.size() > 1) ||
                (child->n_type_ == RouterNodeType::CATCH_ALL && sub_path.size() > 0))
            {
                if (child->n_type_ == RouterNodeType::PARAM)
                {
                    param_values[child->path_.substr(1, child->path_.size()-1)] = sub_path;
                }
                Node *found = FindNode(child, path, next_slash + 1, end, param_values);
                if (found){
                    return found;
                }
            }
        }
        return nullptr;
    }
    else if (parent->children_.find(first_char) != parent->children_.end() &&
             ((parent->children_[first_char]->n_type_ == RouterNodeType::STATIC && parent->children_[first_char]->path_ == sub_path) ||
              (parent->children_[first_char]->n_type_ == RouterNodeType::PARAM && sub_path.size() > 1) ||
              (parent->children_[first_char]->n_type_ == RouterNodeType::CATCH_ALL && sub_path.size() > 0)))
    {
        if (parent->children_[first_char]->n_type_ == RouterNodeType::PARAM)
        {
            param_values[parent->children_[first_char]->path_.substr(1, parent->children_[first_char]->path_.size() - 1)] = sub_path;
        }
        return FindNode(parent->children_[first_char], path, next_slash + 1, end, param_values);
    }
    return nullptr;
}

RouterTireTree::RouterTireTree(){
    root_ = new Node();
    root_->path_ = "";
    root_->wild_child_ = false;
    root_->n_type_ = RouterNodeType::ROOT;
    root_->handler_ = nullptr;
    root_->indices_ = "";
}

RouterTireTree::~RouterTireTree(){
    std::vector<Node*> nodes_to_del;
    nodes_to_del.push_back(root_);
    while(!nodes_to_del.empty()) {
        Node *node = nodes_to_del.back();
        nodes_to_del.pop_back();
        for (auto &child : node->children_){
            nodes_to_del.push_back(child.second);
        }
        delete node;
    }
}

void RouterTireTree::AddRoute(const std::string& path, HttpHandler handler){
    InsertNode(root_, path, 0, path.size(), handler);
}

bool RouterTireTree::HandleRequest(const std::string& path, HttpRequest& req, HttpResponse& res){
    std::unordered_map<std::string, std::string> param_values;
    Node* node = FindNode(root_, path, 0, path.size(), param_values);
    req.params_ = param_values;
    if(node){
        node->handler_(req, res);
        return true;
    }

    return false;
}

HttpRouter::HttpRouter(){
    tree_[MethodType::GET] = RouterTireTree();
    tree_[MethodType::POST] = RouterTireTree();
}

void HttpRouter::AddRoute(MethodType method, const std::string &path, HttpHandler handler){
    tree_[method].AddRoute(path, handler);
}

void HttpRouter::Get(const std::string& path, HttpHandler handler){
    AddRoute(MethodType::GET, path, handler);
}

void HttpRouter::Post(const std::string& path, HttpHandler handler){
    AddRoute(MethodType::POST, path, handler);
}

bool HttpRouter::HandleRequest(const std::string& path, HttpRequest& req, HttpResponse& res){
    if(req.method_ == "GET"){
        return tree_[MethodType::GET].HandleRequest(path, req, res);
    }else if(req.method_ == "POST"){
        return tree_[MethodType::POST].HandleRequest(path, req, res);
    }else{
        return false;
    }
}

}