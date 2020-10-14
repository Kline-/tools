#!/usr/bin/python3

import argparse
import collections
import os

parser = argparse.ArgumentParser(description='Display the top N domain queries and clients.')
parser.add_argument('limit', nargs='?', default=50, type=int, help='maximum number of unique results to display')
args = parser.parse_args()

logdir = '/var/log/bind/'
clients, domains = [], []

for file in os.listdir(logdir):
	if file.startswith('query'):
		with open(logdir + file, 'r') as fd:
			for line in fd.read().split('\n'):
				if 'query:' not in line: continue

				spl = line.split()
				clients.append(spl[6].split('#')[0])
				domains.append(spl[11])

dc = collections.Counter(domains).most_common(args.limit)
print(f'Top {args.limit} Domains')
for k, v in dc:
	print(f'  {v:6}\t{k}')

cc = collections.Counter(clients).most_common(args.limit)
print(f'\nTop {args.limit} Clients')
for k, v in cc:
	print(f'  {v:6}\t{k}')
