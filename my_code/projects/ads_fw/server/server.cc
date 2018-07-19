// -*- C++ -*-
// =============================================================================
/**
 *	Copyright (c) Freewheel, 2007-2009. All rights reserved.
 *
 *	@file
 *
 *	@autho
 *
 * 	@brief
 *
 *	Revision History:
 *			2018/05/03	Created
 */
// =============================================================================
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char**argv)
{
	typedef std::shared_ptr<ads::Sig_Action> Sig_Action_Ptr;
	
	int retval = 0;
	bool run_as_server = false;

	//instantiate singletons
	(void)Ads_Log_Service::instance();
	(void)Ads_Pusher_Service::instance();

	evthread_use_pthreads();
	
	curl_global_init(CURL_GLOBAL_ALL);
	init_openssl_locks();

	CrcGenerateTable();
	cout << "server::start()" << endl;
	cout << "server::end()" << endl;
	return 0;
}
