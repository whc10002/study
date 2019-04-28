// -*- C++ -*-
// =============================================================================
/**
 *	Copyright (c) , 2018-2025. All rights reserved.
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
#include <memory>

using namespace std;

int main(int argc, char**argv)
{
	typedef std::shared_ptr<ads::Sig_Action> Sig_Action_Ptr;
	
	int retval = 0;
	bool run_as_server = false;

	evthread_use_pthreads();
	
	curl_global_init(CURL_GLOBAL_ALL);
	init_openssl_locks();

	CrcGenerateTable();
	cout << "server::start()" << endl;
	cout << "server::end()" << endl;
	return 0;
}
