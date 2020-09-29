# !/usr/bin/env python
# -*- coding: utf-8 -*-

from elasticsearch import  Elasticsearch
from elasticsearch.serializer import JSONSerializer
from datetime import datetime

class SetEncoder(JSONSerializer):
    def default(self, obj):
        if isinstance(obj, set):
            return list(obj)
        if isinstance(obj, Something):
            return 'CustomSomethingRepresentation'
        return JSONSerializer.default(self, obj)

# es = Elasticsearch(serializer=SetEncoder())

# Connect Elastic Search
es = Elasticsearch(
    ['elkes.stg.ss.aws.fwmrm.net'], # elkes.dev.aws.fwmrm.net
    http_auth=('readonly', 'fwreadonly'),
    port=9200,
)
print(es.info())

# Get all Index
#for index in es.indices.get('*'):
#	print index

for index in es.indices.get('fw-ads-external-transaction*'):
	print index

# Data search
res = es.count(index='fw-ads-external-transaction-2020.09', body = {
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
					"range":
					{
						"@timestamp":
						{
							"gte": 1600589511491,
							"lte": 1601194311491,
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
})
print(res)

res = es.search(index='fw-ads-external-transaction-2020.09', size=0, body = {
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
					"range":
					{
						"@timestamp":
						{
							"gte": 1600589511491,
							"lte": 1601194311491,
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
})

print(res)
res = es.count(index='fw-ads-external-transaction-2020.09', body = {
	"query": {
		"match_all": {}
	}
})
print(res)

#print(res)
