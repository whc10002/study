// -*- C++ -*-
//=============================================================================
/**
 *      Copyright (c) , 2018-2025. All rights reserved.
 *
 *      @file	embedded_lighttpd.cc
 *
 *      @author
 *
 *      @brief
 *
 *      Revision History:
 *
 */
//=============================================================================

#include "embedded_lighttpd.h"

#if !defined(ADS_ENABLE_MACOSX)
#include <pthread.h>
#endif

extern "C"
{
	extern int lighttpd_server_open (const char* cf, int instance);
	extern int lighttpd_server_loop (int instance);
	extern int lighttpd_server_close (int instance);
	extern int lighttpd_server_shutdown ();
	extern void lighttpd_server_handle_sighup(int instance);
	extern void lighttpd_server_handle_sigalarm(int instance);
};

Ads_Embedded_Lighttpd::Request::~Request()
{
	if (this->request_) delete this->request_;
}

void
Ads_Embedded_Lighttpd::handle_sighup()
{
	for (int i = 0; i < Ads_Embedded_Lighttpd::instance()->n_instances_; ++i)
		lighttpd_server_handle_sighup(i);
}

void
Ads_Embedded_Lighttpd::handle_sigalarm()
{
	for (int i = 0; i < Ads_Embedded_Lighttpd::instance()->n_instances_; ++i)
		lighttpd_server_handle_sigalarm(i);
}

Ads_Embedded_Lighttpd::Ads_Embedded_Lighttpd()
	: started_(false), busy_(false), n_instances_(1)
	, num_pending_requests_(0)
	, num_selector_threads_(1), num_slow_selector_threads_(1), num_slow_request_threads_(0), num_system_handler_threads_(0)
	, num_slow_requests_(0), max_num_slow_requests_(size_t(-1))
{
	n_threads_ = 0;
}

Ads_Embedded_Lighttpd::~Ads_Embedded_Lighttpd()
{
}

Ads_Embedded_Lighttpd *
Ads_Embedded_Lighttpd::instance()
{
	return ads::Singleton<Ads_Embedded_Lighttpd>::instance();
}

#include "cajun/json/elements.h"

int
Ads_Embedded_Lighttpd::Stats::to_json(json::ObjectP& obj)
{
	mutex_.acquire();

	obj["request"] = json::Number(n_request_);
	obj["bytes_in"] = json::Number(n_bytes_in_);
	obj["bytes_out"] = json::Number(n_bytes_out_);
	obj["request_malformed"] = json::Number(n_request_malformed_);
	obj["request_filtered"] = json::Number(n_request_filtered_);
	obj["request_unsupported"] = json::Number(n_request_unsupported_);
	obj["request_rescheduled"] = json::Number(n_request_rescheduled_);
	obj["request_failed"] = json::Number(n_request_failed_);
	obj["request_ad_u"] = json::Number(n_request_ad_u_);

	if (n_request_smsi_ > 0) obj["request_smsi"] = json::Number(n_request_smsi_);
	if (n_request_smsi_failed_ > 0) obj["request_smsi_failed"] = json::Number(n_request_smsi_failed_);
	if (n_request_scte_ > 0) obj["request_scte"] = json::Number(n_request_scte_);
	if (n_request_scte_failed_ > 0) obj["request_scte_failed"] = json::Number(n_request_scte_failed_);
	if (n_request_scte_notify_ > 0) obj["request_scte_notify"] = json::Number(n_request_scte_notify_);
	if (n_request_scte_notify_failed_ > 0) obj["request_scte_notify_failed"] = json::Number(n_request_scte_notify_failed_);

	mutex_.release();

	return 0;
}

int 
Ads_Embedded_Lighttpd::update_stats(json::ObjectP &obj)
{
		time_t now = ::time(NULL);
		if (stats_per_second_.timestamp_ >= now)
			 return 0;	

		stats_.mutex_.acquire();
		stats_per_second_.mutex_.acquire();

		size_t n_seconds = now - stats_per_second_.timestamp_;
		stats_per_second_.timestamp_ = now;

		stats_per_second_.n_request_ = stats_.n_request_ / n_seconds; stats_.n_request_ = 0;
		stats_per_second_.n_bytes_in_ = stats_.n_bytes_in_ / n_seconds; stats_.n_bytes_in_ = 0;
		stats_per_second_.n_bytes_out_ = stats_.n_bytes_out_ / n_seconds; stats_.n_bytes_out_ = 0;
		stats_per_second_.n_request_malformed_ = stats_.n_request_malformed_ / n_seconds; stats_.n_request_malformed_ = 0;
		stats_per_second_.n_request_filtered_ = stats_.n_request_filtered_ / n_seconds; stats_.n_request_filtered_ = 0;
		stats_per_second_.n_request_unsupported_ = stats_.n_request_unsupported_ / n_seconds; stats_.n_request_unsupported_ = 0;
		stats_per_second_.n_request_rescheduled_ = stats_.n_request_rescheduled_ / n_seconds; stats_.n_request_rescheduled_ = 0;
		stats_per_second_.n_request_failed_ = stats_.n_request_failed_ / n_seconds; stats_.n_request_failed_ = 0;

		stats_per_second_.n_request_smsi_ = stats_.n_request_smsi_ / n_seconds; stats_.n_request_smsi_ = 0;
		stats_per_second_.n_request_smsi_failed_ = stats_.n_request_smsi_failed_ / n_seconds; stats_.n_request_smsi_failed_ = 0;
		stats_per_second_.n_request_scte_ = stats_.n_request_scte_ / n_seconds; stats_.n_request_scte_ = 0;
		stats_per_second_.n_request_scte_failed_ = stats_.n_request_scte_failed_ / n_seconds; stats_.n_request_scte_failed_ = 0;
		stats_per_second_.n_request_scte_notify_ = stats_.n_request_scte_notify_ / n_seconds; stats_.n_request_scte_notify_ = 0;
		stats_per_second_.n_request_scte_notify_failed_ = stats_.n_request_scte_notify_failed_ / n_seconds; stats_.n_request_scte_notify_failed_ = 0;
		stats_per_second_.n_request_ad_u_ = stats_.n_request_ad_u_ / n_seconds; stats_.n_request_ad_u_ = 0;

		stats_per_second_.mutex_.release();
		stats_.mutex_.release();
		
		stats_per_second_.to_json(obj);

		obj["pending_requests"] = json::Number(this->num_pending_requests());

		return 0;
}

int
Ads_Embedded_Lighttpd::get_response(Response *& res, int instance)
{
	res = 0;

	ads::Guard __g(this->mutex_); 
	if (responses_[instance].empty()) return -1;
	res = responses_[instance].front();
	responses_[instance].pop_front();

	void *conn = res->connection_;
	Connection_Map::iterator it = connections_.find(conn);
	if (it == connections_.end())
	{
		ADS_DEBUG((LP_DEBUG, "connection %08x not found\n", conn));
		res->connection_ = 0;
	}
	else
	{
		if (it->second.second != res)
		{
			ADS_DEBUG((LP_DEBUG, "connection %08x, request %08x, response %08x not matched with response %08x (possibly abandoned).\n", conn, it->second.first, it->second.second, res));
		}
		else
		{
			connections_.erase(it);
		}
	}

	return 0;
}

int
Ads_Embedded_Lighttpd::put_response(Response *res, bool is_front)
{
	if (res->content_)
	{
		Ads_Server::instance()->stats(Ads_Server::HTTPD_BYTES_OUT, res->content_->length());
	}

	int instance = res->instance_;
	{
		ads::Guard __g(this->mutex_); 
		if (is_front)
			responses_[instance].push_front(res);
		else
			responses_[instance].push_back(res);
	}

	const char dummy[] = { 0x00 };
	::write(this->notification_pipe_[instance][1], dummy, 1);

	return 0;
}

int
Ads_Embedded_Lighttpd::put_request(Request *req)
{
	Ads_Server::instance()->stats(Ads_Server::HTTPD_REQUESTS);
	if (req->content_)
	{
		Ads_Server::instance()->stats(Ads_Server::HTTPD_BYTES_IN, req->content_->length());
	}

	size_t n = 0;

	{
		ads::Guard __g(this->mutex_); 
		connections_[req->connection_] = std::make_pair(req, (Response *)0);
	}

	{
		ads::Guard __g(this->conditions_[req->type_].mutex_); 
		requests_[req->type_].push_back(req);

		n = (++ this->num_pending_requests_);
		this->conditions_[req->type_].cond_.signal();
	}

	if (Ads_Server::config()->mode_ == Ads_Server_Config::DOT)
		return 0;

	if (!Ads_Server::instance()->is_dot_mode_enabled())
	{
		Ads_Server::Repository_Guard __g(Ads_Server::instance());
		size_t high_watermark = Ads_Server::config()->selector_max_pending_requests_.second;
		if (high_watermark == size_t(-1))
			high_watermark = __g.repository()->system_config().max_pending_requests_.second;

		if (n > high_watermark)
		{
			ADS_LOG((LP_INFO, "enabling dot mode (#req = %d).\n", n));
			Ads_Server::instance()->enable_dot_mode(true);
		}
	}
	else
	{
		Ads_Server::Repository_Guard __g(Ads_Server::instance());
		size_t low_watermark = Ads_Server::config()->selector_max_pending_requests_.first;
		if (low_watermark == size_t(-1))
			low_watermark = __g.repository()->system_config().max_pending_requests_.first;

		if (n < low_watermark)
		{
			ADS_LOG((LP_INFO, "quitting dot mode (#req = %d).\n", n));
			Ads_Server::instance()->enable_dot_mode(false);
		}
	}

	return 0;
}

void
Ads_Embedded_Lighttpd::remove_connection(void *conn)
{
	ads::Guard __g(this->mutex_); 
	Connection_Map::iterator it = connections_.find(conn);
	if (it == connections_.end())
	{
	}
	else
	{
		ADS_DEBUG((LP_DEBUG, "connection %08x, request %08x, response %08x removed\n", conn, it->second.first, it->second.second));
		connections_.erase(it);
	}
}

bool
Ads_Embedded_Lighttpd::has_connection(void *conn) 
{
	ads::Guard __g(this->mutex_); 
	return this->connections_.find(conn) != this->connections_.end();
}

int
Ads_Embedded_Lighttpd::handle_request(Ads_Selection_Context *ctx, Response *&hres)
{
	Ads_Request *req = 0;
	Ads_Response *res = ctx->response_;
	ADS_ASSERT(res);

	if (!res) return -1;

	bool detached = ctx->is_detached();
	bool expired = ctx->is_expired();
	bool failed = false;

	if (expired)
	{
		hres = (Response *)res->arg();
		ADS_ASSERT(hres);

		if (hres)
		{
			hres->content_ = std::make_shared<Ads_String>("timeout");
			hres->status_code_ = 500;
		}

		ctx->destroy();
		return 0;
	}

	if (!(ctx->playlist_ && ctx->playlist_->ready_) && !ctx->needs_external_candidates())
	{
		Stats stats;

		Ads_Server::Repository_Guard __g(Ads_Server::instance());
		Ads_Selector_Base::instance()->activate_selection(__g.repository(), __g.asset_repository(), req, res, ctx);
		{
			int ret = Ads_Selector_Base::instance()->select(ctx);
			if( ret < 0)
			{
				ADS_DEBUG((LP_ERROR, "failed to serve request\n"));
				failed = true;
				++stats.n_request_failed_;
			}
			else if ( ret > 0 )
				detached = true;
		}

		++stats.n_request_;

		if (res->format() == Ads_Response::LEGACY && res->is_empty())
			Ads_Selector_Base::instance()->dot(__g.repository(), req, res);

		this->stats_.merge(stats);
	}

	if (detached && ctx->needs_external_creatives())
	{
		ADS_ASSERT(!ctx->external_ad_candidates_.empty());
		ctx->start_load_external_creatives();
		return 0;
	}

	if (detached && ctx->needs_external_notifications())
	{
		ctx->start_load_external_notifications();
		return 0;
	}

	if (detached && ctx->needs_external_candidates())
	{
		ADS_ASSERT(!ctx->external_ad_candidates_.empty());
		ctx->start_load_external_candidates();
		return 0;
	}

	if(detached && ctx->needs_user_info())
	{
		ctx->start_load_user_info();
		return 0;
	}

	if (!failed && detached && ctx->playlist_)
	{
		req = ctx->request_;
		if (!ctx->playlist_->ready_)
		{
			ctx->playlist_->merge(ctx);
			if (ctx->playlist_->has_ads())
			{
				ctx->playlist_->start_load_ads(ctx);
				return 0;
			}
		}

		res->rep()->playlist_content_ = ctx->playlist_->build(ctx);
		res->rep()->playlist_live_ = req->rep()->live_;
		
		if(ctx->playlist_->playlist_type_ == Ads_Playlist::M3U8)
		{
			Ads_Playlist_M3U8 * m3u8 = reinterpret_cast<Ads_Playlist_M3U8*>(ctx->playlist_);
			if(req->playlist_history_.find(m3u8->playlist_id_) != req->playlist_history_.end())
				req->playlist_history_[m3u8->playlist_id_].duration_left_ = m3u8->duration_left_;
			if (req->update_playlist_history_) 
			{
				req->store_playlist_history(res->playlist_history_);
				res->playlist_id_ = m3u8->playlist_id_;
				if(res->playlist_history_.empty()) res->playlist_history_ = "!";
			}
		}
		if(!ctx->playlist_headers_.empty() && req->rep() && req->rep()->capabilities_.transparent_proxy_mode_) res->playlist_headers_.insert(ctx->playlist_headers_.begin(), ctx->playlist_headers_.end());
	}

	if (detached && res->format() == Ads_Response::TS)
	{
		ctx->mlbam_select_fill_and_log(req, res);
	} 

	res->build_cookies();
	if (res->build() < 0)
    {
		ADS_DEBUG((LP_ERROR, "failed to build response\n"));
		failed = true;
		Ads_Server::instance()->stats(Ads_Server::RESPONSE_BUILD_FAILED);
    }
    if (failed) Ads_Server::instance()->stats(Ads_Server::REQUEST_SERVE_FAILED);

	hres = (Response *)res->arg();
	ADS_ASSERT(hres);

	if (hres)
	{
		res->build_headers(hres->headers_);
		hres->content_ = res->content();
		hres->status_code_ = res->status_code();
	}

	if (Ads_Server::instance()->is_request_tracking_allowed() && req && req->is_request_tracking_enabled())
	{
		Ads_Access_Log_Record *r = new Ads_Access_Log_Record();
		Ads_Selector_Base::instance()->track_request(ctx, r);
	}

	if(req && !req->detached())
	{
		delete req;
		ctx->request_ = nullptr;
	}

	if(res && !res->detached())
	{
		delete res;
		ctx->response_ = nullptr;
	}

	ctx->destroy();

	return 0;
}

int
Ads_Embedded_Lighttpd::handle_request(Request *hreq, Response *&hres, size_t tid)
{
	hres = new Response();
	hres->connection_ =  hreq->connection_;
	hres->instance_ = hreq->instance_;
	{
		ads::Guard __g(this->mutex_); 
		void * conn = hreq->connection_;
		Connection_Map::iterator it = connections_.find(conn);
		if (it == connections_.end())
		{
			ADS_DEBUG((LP_DEBUG, "connection %08x not found\n", conn));
		}
		else
		{
			if (it->second.first != hreq)
			{
				ADS_DEBUG((LP_DEBUG, "connection %08x, request %08x, response %08x not matched with request %08x (possibly abandoned).\n", conn, it->second.first, it->second.second, hreq));
			}
			else
			{
				it->second.second = hres;
			}
		}
	}

	Ads_Request *req = hreq->request_;
	hreq->request_ = 0;

	Ads_Response *res = new Ads_Response();
	Ads_Request_Handler *handler = hreq->handler_;
	ADS_ASSERT(handler);

	int ret = -1;
	bool detached = false;

	Stats stats;
	bool request_serve_failed = false;
	do
	{
		if (!req->h().cors_origin_.empty())
		{
			res->add_custom_header("Access-Control-Allow-Origin", req->h().cors_origin_ == "null"? "*": req->h().cors_origin_);
			res->add_custom_header("Access-Control-Allow-Credentials", "true");
			res->add_custom_header("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
			res->add_custom_header("Access-Control-Max-Age", "1728000");
			res->add_custom_header("Access-Control-Allow-Headers", "content-type, depth, user-agent, x-file-size, x-requested-with, if-modified-since, x-file-name, cache-control");	
		}

		// CORS OPTIONS
		if(req->h().request_method_ == "OPTIONS")
		{
			res->status_code(200);
			res->content("text/html", "");
			res->build_headers(hres->headers_);
			break;
		}

		if (handler->prepare(req) < 0)
		{
			ADS_DEBUG((LP_ERROR, "failed to prepare request\n"));
			++stats.n_request_failed_;
			request_serve_failed = true;
			break;
		}

		if(req->h().request_method_ == "POST" && hreq->content_ && !hreq->content_->empty())
		{
			stats.n_bytes_in_ += hreq->content_->length();
			req->handle_content(hreq->content_);
		}

		if (Ads_Server::instance()->is_request_filtered(req))
		{
			if (Ads_Server::config()->logger_enable_binary_log_)
			{
				if(handler->name() == "smart" || handler->name() == "simple")
				{
					Ads_Server::Repository_Guard __g(Ads_Server::instance());
					ads::Error_List errors;
					req->parse(__g.repository(), __g.asset_repository(), &errors);
				}

				Ads_Selector_Base::instance()->log_banned_request(req);
			}

			ADS_DEBUG((LP_ERROR, "request filtered\n"));
			res->status_code(403);

			++stats.n_request_filtered_;
			Ads_Server::instance()->stats(Ads_Server::REQUEST_FILTERED);
			break;
		}	

		req->arg(hreq);		

		if (req->detached())
		{
			detached = true;
			ADS_DEBUG((LP_TRACE, "request detached.\n"));

			ads::Guard __g(this->mutex_); 
			if (this->put_slow_request(req, res, handler) >= 0)
				return 0;
		}
		{
				Ads_Server::Repository_Guard __g(Ads_Server::instance());

				Ads_String domain = req->h().host_;
				if (!domain.empty())
				{
					size_t end = domain.find(":");
					if(end != Ads_String::npos) domain = domain.substr(0, end);
					Ads_String host = domain;

					ads::tolower(domain);
					bool valid = __g.repository()->system_config().allowed_domains_.find(domain.c_str()) != __g.repository()->system_config().allowed_domains_.end();
					if (!valid) valid = Ads_Server::is_domain_cname_allowed(__g.repository(), domain);
					if (valid)
					{
						req->domain_ = host;
						ADS_DEBUG((LP_DEBUG, "Matched request domain: %s\n", domain.c_str()));
					}
				}

				int rv = handler->process(__g.repository(), __g.asset_repository(), req, res);
				if (rv < 0)
				{
					ADS_DEBUG((LP_ERROR, "failed to process.\n"));
					request_serve_failed = true;
				}

				switch(req->request_type()) 
				{
				case Ads_Request::SMSI: 
					++stats_.n_request_smsi_; 
					Ads_Server::instance()->stats(Ads_Server::SMSI_TOTAL);
					if(rv < 0)
					{
						++stats_.n_request_smsi_failed_;
						Ads_Server::instance()->stats(Ads_Server::SMSI_FAILED);
					}
					break;
				case Ads_Request::SCTE: 
					++stats_.n_request_scte_; 
					Ads_Server::instance()->stats(Ads_Server::SCTE_TOTAL);
					if (rv < 0)
					{
						++stats_.n_request_scte_failed_;
						Ads_Server::instance()->stats(Ads_Server::SCTE_FAILED);
					}
					break;
				case Ads_Request::SCTE_NOTIFY: 
					++stats_.n_request_scte_notify_; 
					Ads_Server::instance()->stats(Ads_Server::PSN_TOTAL);
					if (rv < 0)
					{
						++stats_.n_request_scte_notify_failed_;
						Ads_Server::instance()->stats(Ads_Server::PSN_FAILED);
					}
					break;
				case Ads_Request::USER:
					++stats_.n_request_ad_u_;
					Ads_Server::instance()->stats(Ads_Server::USER_AD_U_TOTAL);
				
				default: break;
				}
		}

		if (res->detached())
        {
			detached = true;
			ADS_DEBUG((LP_TRACE, "response detached.\n"));

			++stats.n_request_rescheduled_;
			Ads_Server::instance()->stats(Ads_Server::REQUEST_RESCHEDULED);
			break;
		}

		res->build_cookies();
        if (res->build() < 0)
        {
			ADS_DEBUG((LP_ERROR, "failed to build response (Ads_Response::build)\n"));

			++stats.n_request_failed_;
			request_serve_failed = true;
			Ads_Server::instance()->stats(Ads_Server::RESPONSE_BUILD_FAILED);
            break;
        }

		res->build_headers(hres->headers_);
		hres->content_ = res->content();
		if (!detached)
		{
			if (Ads_Server::instance()->is_request_tracking_allowed() && req && req->is_request_tracking_enabled())
			{
				Ads_Access_Log_Record *r = new Ads_Access_Log_Record();
				Ads_Selector_Base::instance()->track_request(req, res, r);
			}
		}
		stats.n_bytes_out_ += hres->content_->length();
		++stats.n_request_;
		ret = 0;
	} while(0);

	if (request_serve_failed) Ads_Server::instance()->stats(Ads_Server::REQUEST_SERVE_FAILED);

	if(req && !req->detached())
		delete req;

	if (!detached)
	{
		hres->status_code_ = res->status_code();
		if (res) delete res;
	}
	else
	{
		ret = 0;
		if (res->detached())
		{
			Ads_Selection_Context *ctx = 0;
			if (res->arg())
			{
				ctx = (Ads_Selection_Context *)res->arg();
				ADS_DEBUG((LP_TRACE, "rescheduling slow selection: %08x.\n", ctx));
				res->arg(hres);
				hres = 0;
			}

			if (ctx)
			{
				ctx->in_slow_selection_ = true;
				if ( !ctx->is_detached())
				{
					Ads_Selector_Base::instance()->put_slow_selection(ctx);
				}
				else if (ctx->needs_playlist())
				{
					ctx->start_load_playlist();
				}
				else if (ctx->needs_external_creatives())
				{
					ctx->start_load_external_creatives();
				}
				else if (ctx->needs_external_notifications())
				{
					ctx->start_load_external_notifications();
				}
				else if (ctx->needs_external_candidates())
				{
					ctx->start_load_external_candidates();
				}
				else if (ctx->needs_user_info() || ctx->needs_user_state())
				{
					ctx->start_load_user_info();
				}
				else
				{
					ADS_LOG((LP_ERROR, "unexpected ctx state\n"));
				}
			}
		}
	}
	this->stats_.merge(stats);
	return ret;
}

int
Ads_Embedded_Lighttpd::open()
{
	this->n_instances_ = Ads_Server::config()->embedded_httpd_num_instances_;

	for (int i = 0; i < n_instances_; ++i)
	{
	if (::pipe(this->notification_pipe_[i]) != 0)
	{
		ADS_LOG((LP_ERROR, "failed to create notification pipe\n"));
		return -1;
	}

	::fcntl (this->notification_pipe_[i][0], F_SETFD, 1);
	::fcntl (this->notification_pipe_[i][1], F_SETFD, 1);

	::fcntl (this->notification_pipe_[i][0], F_SETFL, O_NONBLOCK);
	::fcntl (this->notification_pipe_[i][1], F_SETFL, O_NONBLOCK);
	}
	
	for (int i = 0; i < n_instances_; ++i)
	{
	if (lighttpd_server_open(config_file_.c_str(), i) < 0)
	{
		ADS_LOG((LP_ERROR, "failed to start lighttpd\n"));
		return -1;
	}
	}

	this->num_selector_threads_ = Ads_Server::config()->embedded_httpd_num_selector_threads_;
	this->num_slow_selector_threads_ = Ads_Server::config()->embedded_httpd_num_slow_selector_threads_;
	this->num_slow_request_threads_ = Ads_Server::config()->embedded_httpd_num_slow_request_threads_;
	this->num_system_handler_threads_ = Ads_Server::config()->embedded_httpd_num_system_handler_threads_;

	this->stats_per_second_.timestamp_ = ::time(NULL);

	this->n_threads_ = n_instances_ /* http server */ + this->num_selector_threads_ + this->num_slow_selector_threads_ + this->num_slow_request_threads_ + this->num_system_handler_threads_;

	return Ads_Service_Base::open();
}	

int
Ads_Embedded_Lighttpd::stop()
{
	this->pre_exitting_ = true;
	while (!this->connections_.empty())
	{
		usleep(10000);
	}

	this->exitting_ = true;
	Ads_Selector_Base::instance()->deactivate_slow_selections();
	this->slow_requests_.deactivate();
	for (int i = 0; i < n_instances_; ++i)
	{
		::close(this->notification_pipe_[i][0]);
		::close(this->notification_pipe_[i][1]);
	}

	if (this->started_)
		lighttpd_server_shutdown ();

	for (size_t i = 0; i < Request::REQUEST_LAST + 1; ++i)
	{
		ads::Guard __g(this->conditions_[i].mutex_); 
		this->conditions_[i].cond_.broadcast();
	}

	this->wait();

	if (this->started_)
	{ 
		for (int i = 0; i < n_instances_; ++i)
			lighttpd_server_close(i);
	}

	return 0;
}

int
Ads_Embedded_Lighttpd::svc_slow_select(size_t)
{
		while (!this->exitting_)
		{
			if (this->busy_) { sleep(1); continue; }

			Ads_Selection_Context *ctx = 0;
			if (Ads_Selector_Base::instance()->get_slow_selection(ctx) < 0 || !ctx)
				continue;

			if (this->exitting_) break;

			Response *hres = 0;
			Ads_Log_Service::instance()->set_transaction_id(ctx->transaction_id_);
			if (this->handle_request(ctx, hres) < 0)
			{
				Ads_Log_Service::instance()->clear_transaction_id();
				continue;
			}
			Ads_Log_Service::instance()->clear_transaction_id();

			if (hres)
				this->put_response(hres);
		}
	
	return 0;
}

int
Ads_Embedded_Lighttpd::svc_slow_request(size_t)
{
	while (!this->exitting_)
	{
		if (this->busy_) { sleep(1); continue; }
		Ads_Request *req = 0;
		Ads_Response *res = 0;
		Ads_Request_Handler *handler = 0;

		this->get_slow_request(req, res, handler);

		if (this->exitting_) break;
		if (!req || !res || !handler) continue;

		req->detached(false);
		res->detached(false);

		Stats stats;
		Ads_Log_Service::instance()->set_transaction_id(req->transaction_id());

		{
				Ads_Server::Repository_Guard __g(Ads_Server::instance());

				if(handler->process(__g.repository(), __g.asset_repository(), req, res) < 0)
				{
					ADS_DEBUG((LP_ERROR, "failed to process.\n"));
				}
		}

		ADS_ASSERT (!res->detached());

		res->build_cookies();
        if (res->build() < 0)
        {
			ADS_DEBUG((LP_ERROR, "failed to build response (Ads_Response::build)\n"));

			++stats.n_request_failed_;
        }
		Ads_Log_Service::instance()->clear_transaction_id();

		Response *hres = (Response *)res->arg();
		res->build_headers(hres->headers_);
		hres->content_ = res->content();

		hres->status_code_ = res->status_code();

		stats.n_bytes_out_ += hres->content_->length();
		++stats.n_request_;

		if (hres)
			this->put_response(hres);

		delete req;
		delete res;

		this->stats_.merge(stats);
	}
	
	return 0;
}
#endif

int Ads_Embedded_Lighttpd::svc_select(size_t i) { return this->svc_i(i, Request::REQUEST_SELECTION); }
int Ads_Embedded_Lighttpd::svc_system(size_t i) { return this->svc_i(i, Request::REQUEST_SYSTEM); }

int
Ads_Embedded_Lighttpd::svc_i(size_t i, Request::TYPE t)
{
		size_t sequence_id = 0;
		while (!this->exitting_)
		{
			if (this->busy_) { sleep(1); continue; }
			Request *req = 0;
			Response *res = 0;

			this->conditions_[t].mutex_.acquire();
			while (this->requests_[t].empty() && !this->exitting_)
				this->conditions_[t].cond_.wait();

			if (!this->exitting_)
			{
				req = this->requests_[t].front();
				this->requests_[t].pop_front();

				--num_pending_requests_;
			}

			this->conditions_[t].mutex_.release();

			if (this->exitting_)
			{
				if (req) req->destroy();
				break;
			}

			if (req == nullptr || req->request_ == nullptr) continue;

			req->request_->thread_id(i);
			req->request_->sequence_id(sequence_id++);
			Ads_Log_Service::instance()->set_transaction_id(req->request_->transaction_id());
			this->handle_request(req, res, i);
			req->destroy();
			Ads_Log_Service::instance()->clear_transaction_id();
			
			if (res)
				this->put_response(res, (t == Request::REQUEST_SYSTEM) );
		}

	return 0;
}

int
Ads_Embedded_Lighttpd::svc()
{
	int ret = 0;
	pthread_t tid = ads::thr_self();
	std::pair<size_t, MODE> r = this->thread_mode(tid);
	MODE mode = r.second;
	size_t i = r.first;

	if(mode == HTTP_SERVER)
	{
		int instance = (int)i;
		this->started_ = true;
		ADS_LOG((LP_INFO, "(http: %llu) service (http_server) started.\n", ads::thr_id()));

		cpu_set_t cpuset;
		pthread_t tid = pthread_self(); 
		int core = instance;
		CPU_ZERO(&cpuset);  
		CPU_SET(core, &cpuset);  
		pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpuset);
		ret = lighttpd_server_loop(instance);

		ADS_LOG((LP_INFO, "(http: %llu) service (http_server) stopped.\n", ads::thr_id()));
		return ret;
	}
	else if (mode == SLOW_AD_SELECTOR)
	{
		ADS_LOG((LP_INFO, "(http: %llu) service (slow_selector/%u) started.\n", ads::thr_id(), i));

		this->svc_slow_select(i);
	
		ADS_LOG((LP_INFO, "(http: %llu) service (slow_selector/%u) stopped.\n", ads::thr_id(), i));
	}
	else if (mode == SLOW_REQUEST)
	{
		ADS_LOG((LP_INFO, "(http: %llu) service (slow_request/%u) started.\n", ads::thr_id(), i));

		this->svc_slow_request(i);
	
		ADS_LOG((LP_INFO, "(http: %llu) service (slow_request/%u) stopped.\n", ads::thr_id(), i));
	}
	else
	{
		const char *ms = mode == AD_SELECTOR? "selector" : "system";
		ADS_LOG((LP_INFO, "(http: %llu) service (%s/%u) started.\n", ads::thr_id(), ms, i));

		if (mode == AD_SELECTOR) this->svc_select(i);
		else this->svc_system(i);

		ADS_LOG((LP_INFO, "(http: %llu) service (%s/%u) stopped.\n", ads::thr_id(), ms, i));
	}

	return ret;
}

extern "C"
{
#include "3rd/lighttpd/async_api.h"
};

void *lighttpd_plugin_create_request(void *connection, const char *uri, int instance)
{
	if (Ads_Embedded_Lighttpd::instance()->has_connection(connection))
	{
		ADS_LOG((LP_INFO, "connection %08x uri %s is already in processing.\n", connection, uri? uri: ""));
		return 0;
	}

	Ads_Request_Handler *handler = 0;
	if (!Ads_Server::instance()->request_parser().is_request_interested(uri, &handler) || !handler)
	{
		ADS_LOG((LP_INFO, "connection %08x uri %s not interested.\n", connection, uri? uri: ""));
		return 0;
	}

	Ads_Embedded_Lighttpd::Request *r = new Ads_Embedded_Lighttpd::Request();
	r->time_created_ = ::time(NULL);
	r->connection_ = connection;
	r->type_ = (handler && handler->type_ > 0) ? Ads_Embedded_Lighttpd::Request::REQUEST_SYSTEM : Ads_Embedded_Lighttpd::Request::REQUEST_SELECTION;
	r->request_ = new Ads_Request();
	r->handler_ = handler;
	r->instance_ = instance;

	return r;
}

void lighttpd_plugin_destroy_request(void *req)
{
	Ads_Embedded_Lighttpd::Request *r = reinterpret_cast<Ads_Embedded_Lighttpd::Request *>(req);
	if (r) delete r;
}

void *lighttpd_plugin_request_get_connection(void *req)
{
	Ads_Embedded_Lighttpd::Request *r = reinterpret_cast<Ads_Embedded_Lighttpd::Request *>(req);
	if (r) return r->connection_;
	return 0;
}

void lighttpd_plugin_remove_connection(void *conn)
{
	Ads_Embedded_Lighttpd::instance()->remove_connection(conn);
}

int lighttpd_plugin_add_request_header(void *req, const char *key, const char *val, int is_http)
{
	Ads_Embedded_Lighttpd::Request *r = reinterpret_cast<Ads_Embedded_Lighttpd::Request *>(req);
	if (!r) return -1;

	Ads_String k, v;
	if (key) k = key;
	if (val) v = val;
	if (is_http)
	{
		k = Ads_String("HTTP_") + k;
		for (size_t i = 0; i < k.length(); ++i) if (k[i] == '-') k[i] = '_';

		ads::toupper(k);
	}

	if (r->request_)
		r->request_->handle_header(k, v);
	r->headers_.push_back(std::make_pair(key, val));
	return 0;
}

int lighttpd_plugin_prepare_request_content(void *req, size_t len)
{
	Ads_Embedded_Lighttpd::Request *r = reinterpret_cast<Ads_Embedded_Lighttpd::Request *>(req);
	if (!r) return -1;
	
	r->content_.reset(new Ads_String());
	r->content_->reserve(len);
	return 0;
}

int lighttpd_plugin_add_request_content(void *req, const char *data, size_t len)
{
	Ads_Embedded_Lighttpd::Request *r = reinterpret_cast<Ads_Embedded_Lighttpd::Request *>(req);
	if (!r || !r->content_) return -1;
	
	r->content_->append(data, len);
	return 0;
}

int lighttpd_plugin_enqueue_request(void *req)
{
	if (Ads_Embedded_Lighttpd::instance()->pre_exitting_)
		return -1;
	Ads_Embedded_Lighttpd::Request *r = reinterpret_cast<Ads_Embedded_Lighttpd::Request *>(req);
	if (!r) return -1;
	
	Ads_Embedded_Lighttpd::instance()->put_request(r);
	return 0;
}

int lighttpd_plugin_dequeue_response(void **res, int instance)
{
	*res = 0;

	Ads_Embedded_Lighttpd::Response *r = 0;
	if (Ads_Embedded_Lighttpd::instance()->get_response(r, instance) < 0 || !r)
		return -1;

	*res = r;
	return 0;
}

void lighttpd_plugin_destroy_response(void *res)
{
	Ads_Embedded_Lighttpd::Response *r = reinterpret_cast<Ads_Embedded_Lighttpd::Response *>(res);
	if (r) delete r;
}

void *lighttpd_plugin_response_get_connection(void *res)
{
	Ads_Embedded_Lighttpd::Response *r = reinterpret_cast<Ads_Embedded_Lighttpd::Response *>(res);
	if (r) return r->connection_;
	return 0;
}

int lighttpd_plugin_response_get_status(void *res)
{
	Ads_Embedded_Lighttpd::Response *r = reinterpret_cast<Ads_Embedded_Lighttpd::Response *>(res);
	if (r) return r->status_code_;
	return -1;
}

size_t lighttpd_plugin_response_get_num_headers(void *res)
{
	Ads_Embedded_Lighttpd::Response *r = reinterpret_cast<Ads_Embedded_Lighttpd::Response *>(res);
	if (r) return r->headers_.size();
	return 0;
}

int lighttpd_plugin_response_get_header(void *res, size_t i, const char **key, const char **val)
{
	Ads_Embedded_Lighttpd::Response *r = reinterpret_cast<Ads_Embedded_Lighttpd::Response *>(res);
	if (!r)
		return -1;
	if (i >= r->headers_.size())
		return -1;

	*key = r->headers_[i].first.c_str();
	*val = r->headers_[i].second.c_str();

	return 0;
}

int lighttpd_plugin_response_get_data(void *res, const char **data, size_t *len, int *compress_encoding_mask)
{
	*data =  0;
	*len = 0;

	Ads_Embedded_Lighttpd::Response *r = reinterpret_cast<Ads_Embedded_Lighttpd::Response *>(res);
	if (!r || !r->content_) return -1;

	if (!r->content_) return -1;

	*data = r->content_->c_str();
	*len = r->content_->length();

	return 0;
}
 
int lighttpd_plugin_notification_fd(int instance)
{
	return Ads_Embedded_Lighttpd::instance()->notify_handle(instance);
}

int lighttpd_plugin_request_is_interested(const char *uri)
{
	return (Ads_Server::instance()->request_parser().is_request_interested(uri, 0))? 1 : 0;
}

void lighttpd_plugin_write_accesslog(int fd, const char *data, size_t len)
{
	Ads_Log_Service::instance()->log_httpd(fd, data, len);	
}

