#!/usr/bin/env python

from htmlmin import main as htmlmin
import codecs
import os

base_path = 'arduino/WifiWebServer/'
html_folder = 'html'

pages = os.listdir(os.path.join(base_path, html_folder))

for page in pages:
	path = os.path.join(base_path, html_folder, page)

	if page.endswith('.html') and os.path.isfile(path):
	    with codecs.open(path, encoding='utf-8') as f:
			html = f.read()
			ohtml = htmlmin.minify(html).replace('\n', '\\\n').replace('"', '\\"')
			page_name = os.path.splitext(page)[0]

			header_path = os.path.join(base_path, '%s.h' % page_name)
			with codecs.open(header_path, encoding='utf-8', mode='w') as of:
				of.write('static const char %s[] = "%s";' % (page_name.upper(), ohtml))
				print 'Create %s' % header_path
