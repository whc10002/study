#!/usr/bin/python
# coding=utf-8

from Elastic_search import *
from Config import *

"""
Run query
"""
def run(param):
    es = init_es(ELASTIC_HOST, ELASTIC_PORT, ELASTIC_USER_NAME, ELASTIC_USER_PW)
    log_date = param['log_date'] if param['log_date'] else datetime.datetime.now().strftime('%Y.%m')
    count = param['count'] if param['count'] else 50
    is_page = param['is_page']
    index_name = "fw-ads-external-transaction-"+log_date

    begin_time = int(param['begin_time'] if param['begin_time'] else (time.time() - 30*24*3600)*1000)
    end_time = int(param['end_time'] if param['end_time'] else time.time()*1000)

    query = format_query(begin_time, end_time)
    total_count = get_total_count(es, index_name, query)
    print("total count:"+str(total_count))
    if not is_page:
        print("no paging")
        query_data(es, index_name, count, query)
    else:
        page_count = int(param['page_count'])
        total_page = int(total_count)/page_count if int(total_count)%page_count == 0 else int(total_count)/page_count + 1
        for page_num in range(0, total_page): 
            query_data_by_page(es, index_name, page_count, page_num, query)

def main(argv):
    try:
        opts, args = getopt.getopt(argv[1:], 'hp:l:c:io:n:',
                                   ['help', 'puid=', 'log_date=', 'count=', 'is_page', 'page_count=', 'page_num='])
    except getopt.GetoptError as err:
        print str(err)
        sys.exit(2)

    if not opts:
        print "The puid is a must !"
        opts = [('-h', '')]

    VARS = {'log_date': None, 'count': None,'is_page': False, 'page_count': 50, 'begin_time': None, 'end_time': None}

    for opt, value in opts:

        if opt in ('-h', '--help'):
            print("")
            print("Usage: python query_client_log.py [-c count -l log_date -i [-o page_count -n page_count]] | --puid=puid ....")
            print("-l, --log_date       date of data:YYYY.mm")
            print("-c, --count          query number")
            print("-i, --is_page        if page")
            print("-o, --page_count     number in every page")
            print("-h, --help           help")
            print("")
            sys.exit()

        if opt in ('-l', '--log_date'):
            VARS['log_date'] = value
        elif opt in ('-c', '--count'):
            VARS['count'] = value
        elif opt in ('-i', '--is_page'):
            VARS['is_page'] = True
        elif opt in ('-o', '--page_count'):
            VARS['page_count'] = value

    run(VARS)

if __name__ == '__main__':
    main(sys.argv)
