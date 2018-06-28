#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<hiredis/hiredis.h>
#include<map>
using namespace std;

struct Test {
	int i;
	int j;
	int k;
};

class Redis{
public:
	Redis(){}
	~Redis(){
		this->_connect = NULL;
		this->_reply = NULL;
	}
	bool connect(std::string host, int port){
		this->_connect = redisConnect(host.c_str(), port);
		if(this->_connect != NULL && this->_connect->err){
			printf("connect error : %s\n",this->_connect->errstr);
			return -1;
		}
		return 1;
	}
	
	//
	void set(string key,string value){
		redisCommand(this->_connect,"SET %s %s",key.c_str(), value.c_str());
		freeReplyObject(this->_reply);
	}

	//key-value
	void/*std::string*/ get(){
			int map=keys();
			printf("%d\n",map);
			for(int i=0 ; i<map ; i++){
				//printf("mapRedis[%d]=  %s\n",i,mapRedis[i].c_str());	
				this->_reply = (redisReply*)redisCommand(this->_connect,"GET %s",mapRedis[i].c_str());
				string str = this->_reply->str;
				cout<<"mapRedis[ "<<i<<" ] = "<<mapRedis[i]<<" values: "<<str<<endl;
			}
			freeReplyObject(this->_reply);
			//return str;
	}
	
	//得到所有的key
	int keys(){
			int map=0;
			this->_reply = (redisReply*)redisCommand(this->_connect,"keys *");
			for (int i = 0; i < this->_reply->elements; ++i) {  
        redisReply* childReply = this->_reply->element[i];  
				//printf("keys[%d]: %s\n", i, childReply->str);
				//mapRedis.insert(map<string,string>::value_type(childReply->str,NULL));
				mapRedis[i]=childReply->str;
				map = mapRedis.size();
    	} 
			freeReplyObject(this->_reply);
			return map;
	}
	//
	void* CreateThread(void* list){
			char *l=(char*)list;
			while(true){
				brpop(l);
			}
			return NULL;
	}
	//
	void brpop(std::string hash){
			this->_reply = (redisReply*)redisCommand(this->_connect,"brpop %s 600",hash.c_str());
			string str = this->_reply->str;
			freeReplyObject(this->_reply);
			
	}
	//读取二进制数据 
	void hget(std::string hash, const string& sid){
			this->_reply = (redisReply*)redisCommand(
					this->_connect,"hget %s %s",hash.c_str(),sid.c_str()
			);
#if 0
			struct Test t;		
			memcpy(&t, _reply->str, sizeof(t));
			cout << "------" << endl;
			cout << t.i << endl;
			cout << t.j << endl;
			cout << t.k << endl;
			cout << "------" << endl;
#endif
			cout << "------" << endl;
			cout << _reply->str << endl;
			cout << "------" << endl;
			//memcpy(session,this->_reply->str,len);
			freeReplyObject(this->_reply);
	}
	
	//写入二进制数据
	void hset(std::string hash, const string& sid, const string& /*ClientSession**/ session /*, const size_t& len*/){
//	void hset(std::string hash, const string& sid, struct Test* t){
			this->_reply = (redisReply*)redisCommand(
//					this->_connect,"hset %s %s %b",hash.c_str(),sid.c_str(),t, sizeof(struct Test)
					this->_connect,"hset %s %s %b",hash.c_str(),sid.c_str(),session.c_str(), session.size()
			);
			freeReplyObject(this->_reply);
	}
	
	int llen(std::string key){
			printf("______________%s_______________\n",key.c_str());
			this->_reply = (redisReply*)redisCommand(this->_connect,"llen %s",key.c_str());
			string str = this->_reply->str;
			freeReplyObject(this->_reply);
			return atoi(str.c_str());
	}
	
private:
	redisContext* _connect;//用于连接
	redisReply* _reply;//收发数据
	map<int,string> mapRedis;
	map<int,string>::iterator iter;
};

class RedisConf{
public:
	RedisConf(){
		getConf();
	}
	~RedisConf(){
		fclose(fd);
		fd = NULL;
	}
	void getConf(){
		fd = fopen("./RedisConf.conf","r");
		if(fd == NULL){
			perror("open file error.\n");
			fclose(fd);	
			fd = NULL;
			return;
		}
	}
	string getIP(){
		buf = NULL;
		//len=0;
		char** p_buf = &buf;
		getline(p_buf,&len,fd);
		_str = strstr(buf,"= ")+2;
		_ip=trim(_str);
		cout<<"_____1_____"<<_ip<<"_________"<<endl;
		free(buf);
		buf = NULL;
		
		return _ip;
	}
	int getPort(){
		buf = NULL;
		len =0;
		getline(&buf,&len,fd);
		_str=strstr(buf,"= ")+2;
		_str=trim(_str);
		_port=atoi(_str.c_str());
		cout<<"____2______"<<_port<<"_________"<<endl;
		return _port;
	}
	string trim(string &s){
		if(s.empty()){
			return s;
		}
		s.erase(s.find_last_not_of(" \n")+1);
		return s;
	}
private:
	FILE   *fd = NULL;
	size_t len = 0;
	string _str;
	string _ip;
	int    _port = 0;
	char   *buf = NULL;
	
	
};

int main(){
	Redis *redis=new Redis();	
	RedisConf redisconf;
	string host = redisconf.getIP();
	int port =redisconf.getPort();
	if(!redis->connect(host,port)){
	//if(!redis->connect(redisconf.getIP(),6380)){
	//if(!redis->connect("47.92.54.192",6380)){
		printf("connect error!\n");
		return -1;
	}
	//redis->get();
	string dell="dell_1";
	string sid="1607_1";
	string session = "1607";
	struct Test t = {10, 20, 30};
	//size_t len=1;
	//redis->hset(dell,sid,session);
	redis->hset(dell,sid,string((char*)&t));
	redis->hget(dell,sid);
	return 0;
}	
