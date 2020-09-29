#!/usr/bin/python
# coding=utf-8

import datetime
import sys
import getopt
import time
from elasticsearch import Elasticsearch

"""
Init Elastic Search Object
"""
def init_es(host, port, user, pw):
    return Elasticsearch(
		[host], 
        http_auth=(user, pw),
		port=int(port),
	)

"""
Query data
"""
def query_data(es, index_name, count, query):
    data_array = es.search(index=index_name, size=int(count), body = query)

    print_data(data_array, count)

"""
Query data by paging
"""
def query_data_by_page(es, index_name, page_count, page_num, query):
    from_num = page_count * page_num

    data_array = es.search(index=index_name, size=int(page_count), from_=from_num, body = query)

    print("get data from:["+str(from_num)+"] to:["+str(from_num+page_count)+"]")
    print_data(data_array, page_count)

"""
Get match total number
"""
def get_total_count(es, index_name, query):
	return int(es.count(index=index_name, body = query)['count'])

"""
Print data
"""
def print_data(data_array, count):

    datas = data_array["hits"]["hits"]

    print "" + str(data_array['hits']['total'])
    print ""
    for data in datas:
        print data['_source']['external-transaction']['url']
        print data['_source']['external-transaction']['response'][1:6]

"""
Format query
"""
def format_query(begin_time, end_time):
	res = {
			"query": 
			{ 
				"bool":
				{
					"must":
					[
						{
							"match_all": {}
						},
						{
							"exists":
							{
								"field": "external-transaction.response"
							}
						},
						{
							"bool":
							{
								"should":
								[
									{
										"match_phrase":
										{
											"external-transaction.response":
											{
												"query": "*VAST*"
											}
										}
									},
									{
										"match_phrase":
										{
											"external-transaction.response":
											{
												"query": "*bidid*" 
											}
										}
									}
								]
							}
						},
						{
							"range":
							{
								"@timestamp":
								{
									"gte": begin_time,
									"lte": end_time,
									"format": "epoch_millis"
								}
							}
						}
					],
					"filter": [],
					"should": [],
					"must_not": []
				}
			}
		}
	return res

