// -*- C++ -*-
//=============================================================================
/**
 *      Copyright (c) , 2018-2025. All rights reserved.
 *
 *      @file	embedded_lighttpd.hpp
 *
 *      @author
 *
 *      @brief
 *
 *      Revision History:
 *
 */
//=============================================================================
#include <atomic>
#include "types.hpp"

#define MAX_INSTANCE 2

class Ads_Embedded_Lighttpd:  public Ads_Service_Base
{
public:
	struct Request
	{
		enum TYPE
		{
			REQUEST_SELECTION = 0,
			REQUEST_SYSTEM = 1,
			REQUEST_LAST = REQUEST_SYSTEM
		};

		time_t time_created_;
		TYPE type_;

		std::vector<std::pair<std::string, std::string> > headers_;
		Ads_Message_Block content_;

		void *connection_;
		int instance_;
		
		Request()
			: time_created_(0)
			, type_(REQUEST_SELECTION)
			, connection_(0)
			, instance_(0)
		{}
		~Request();

		void destroy() { delete this; }
	};

	struct Response
	{
		int status_code_;
		time_t time_created_;

		std::vector<Ads_String_Pair> headers_;
		Ads_Message_Block content_;

		void *connection_;
		int instance_;

		Response()
			:status_code_(200)
			, time_created_(0)
			, connection_(0)
			, instance_(0)
		{}

		~Response()
		{}

		void destroy() { delete this; }
	};

	bool started_;
	bool busy_;
	int n_instances_;

private:
	int notification_pipe_[MAX_INSTANCE][2];

	std::list<Request *> requests_[Request::REQUEST_LAST + 1];
	std::list<Response *> responses_[MAX_INSTANCE];
	std::atomic<int> num_pending_requests_;

	typedef std::map<void *, std::pair<Request *, Response *> > Connection_Map;
	Connection_Map connections_;

	size_t num_selector_threads_, num_slow_selector_threads_, num_slow_request_threads_, num_system_handler_threads_;

public:
	std::string config_file_;

	enum MODE
	{
		HTTP_SERVER,
		AD_SELECTOR,
		SLOW_AD_SELECTOR,
		SLOW_REQUEST,
		SYSTEM_HANDLER
	};

	struct Stats
	{
		time_t timestamp_;
		int n_request_;
		int n_bytes_in_, n_bytes_out_;
		int n_request_malformed_;
		int n_request_filtered_;
		int n_request_unsupported_;
		int n_request_rescheduled_;
		int n_request_failed_;
	
		int n_request_smsi_;
		int n_request_smsi_failed_;
		int n_request_scte_notify_;
		int n_request_scte_notify_failed_;
		int n_request_scte_;
		int n_request_scte_failed_;
		
		int n_request_ad_u_;

		Stats()
			: timestamp_(0)
			, n_request_(0)
			, n_bytes_in_(0), n_bytes_out_(0)
			, n_request_malformed_(0)
			, n_request_filtered_(0)
			, n_request_unsupported_(0)
			, n_request_rescheduled_(0)
			, n_request_failed_(0)
			, n_request_smsi_(0), n_request_smsi_failed_(0)
			, n_request_scte_notify_(0), n_request_scte_notify_failed_(0)
			, n_request_scte_(0), n_request_scte_failed_(0)
			, n_request_ad_u_(0)
		{}
		
		std::mutex mutex_;
		void inc(int& i) 
		{ 
			++i; 
		}

		void inc(int& i, int delta) 
		{ 
			i += delta; 
		}

		void merge(const Stats& s)
		{
			mutex_.acquire();

			n_request_ += s.n_request_;
			n_bytes_in_ += s.n_bytes_in_; 
			n_bytes_out_ += s.n_bytes_out_;
			n_request_malformed_ += s.n_request_malformed_;
			n_request_filtered_ += s.n_request_filtered_;
			n_request_unsupported_ += s.n_request_unsupported_;
			n_request_rescheduled_ += s.n_request_rescheduled_;
			n_request_failed_ += s.n_request_failed_;

			n_request_smsi_ += s.n_request_smsi_;
			n_request_smsi_failed_ += s.n_request_smsi_failed_;
			n_request_scte_ += s.n_request_scte_;
			n_request_scte_failed_ += s.n_request_scte_failed_;
			n_request_scte_notify_ += s.n_request_scte_notify_;
			n_request_scte_notify_failed_ += s.n_request_scte_notify_failed_;
			
			n_request_ad_u_ += s.n_request_ad_u_;

			mutex_.release();
		}

		int to_json(json::ObjectP& obj);
	};

	struct Stats stats_, stats_per_second_;
	int update_stats(json::ObjectP &obj);
	
private:
	int svc_i(size_t i, Request::TYPE);
	int svc_select(size_t i);
	int svc_slow_select(size_t i);
	int svc_slow_request(size_t i);
	int svc_system(size_t i);

	size_t num_slow_requests_;
	size_t max_num_slow_requests_;

public:
	Ads_Embedded_Lighttpd();
	~Ads_Embedded_Lighttpd();
	static Ads_Embedded_Lighttpd *instance();

	int open();
	int svc();
	int stop();

	int put_response(Response *res, bool is_front = false);
	int get_response(Response *&res, int instance);

	int put_request(Request *req) ;
	void remove_connection(void *conn);
	bool has_connection(void *conn);

	int notify_handle(int instance) { return this->notification_pipe_[instance][0]; }

	long num_pending_requests() const { return this->num_pending_requests_.load(); }

	void busy(bool f)	{ busy_ = f; }
	bool busy() const	{ return busy_; }

	std::pair<size_t, MODE> thread_mode(pthread_t tid) const
	{
		size_t i = 0; 
		for (; i < thread_ids_.size(); ++i)
			if (tid == thread_ids_[i]) break;

		if (i < (size_t)n_instances_) return std::make_pair(i, HTTP_SERVER);
		else if (i < n_instances_ + this->num_selector_threads_) return std::make_pair(i, AD_SELECTOR);
		else if (i < n_instances_ + this->num_selector_threads_ + this->num_slow_selector_threads_) return std::make_pair(i, SLOW_AD_SELECTOR);
		else if (i < n_instances_ + this->num_selector_threads_ + this->num_slow_selector_threads_ + this->num_slow_request_threads_) return std::make_pair(i, SLOW_REQUEST);
		else return std::make_pair(std::min(i, thread_ids_.size() - 1), SYSTEM_HANDLER);
	}

	static void handle_sighup();
	static void handle_sigalarm();
	volatile bool pre_exitting_ = false;

private:
	int handle_request(Request *req, Response *& res, size_t tid);
};
