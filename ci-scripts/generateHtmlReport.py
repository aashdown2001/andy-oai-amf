#/*
# * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# * contributor license agreements.  See the NOTICE file distributed with
# * this work for additional information regarding copyright ownership.
# * The OpenAirInterface Software Alliance licenses this file to You under
# * the OAI Public License, Version 1.1  (the "License"); you may not use this file
# * except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *	  http://www.openairinterface.org/?page_id=698
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *-------------------------------------------------------------------------------
# * For more information about the OpenAirInterface (OAI) Software Alliance:
# *	  contact@openairinterface.org
# */
#---------------------------------------------------------------------

import glob
import os
import re
import sys
import subprocess

class HtmlReport():
	def __init__(self):
		self.job_name = ''
		self.job_id = ''
		self.job_url = ''
		self.job_start_time = 'TEMPLATE_TIME'
		self.git_url = ''
		self.git_src_branch = ''
		self.git_src_commit = ''
		self.git_src_commit_msg = None
		self.git_pull_request = False
		self.git_target_branch = ''
		self.git_target_commit = ''
		self.nb_warnings = 0
		self.warning_rows = ''

	def generate(self):
		cwd = os.getcwd()
		self.file = open(cwd + '/test_results_oai_amf.html', 'w')
		self.generateHeader()

		self.coding_formatting_log()

		self.analyze_sca_log()

		self.buildSummaryHeader()
		self.initialGitSetup()
		self.installLibsPackagesRow()
		self.buildCompileRows()
		self.copyToTargetImage()
		self.copyConfToolsToTargetImage()
		self.imageSizeRow()
		self.buildSummaryFooter()

		self.generateFooter()
		self.file.close()

	def generateHeader(self):
		# HTML Header
		self.file.write('<!DOCTYPE html>\n')
		self.file.write('<html class="no-js" lang="en-US">\n')
		self.file.write('<head>\n')
		self.file.write('  <meta name="viewport" content="width=device-width, initial-scale=1">\n')
		self.file.write('  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">\n')
		self.file.write('  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>\n')
		self.file.write('  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>\n')
		self.file.write('  <title>OAI Core Network Test Results for ' + self.job_name + ' job build #' + self.job_id + '</title>\n')
		self.file.write('</head>\n')
		self.file.write('<body><div class="container">\n')
		self.file.write('  <table width = "100%" style="border-collapse: collapse; border: none;">\n')
		self.file.write('   <tr style="border-collapse: collapse; border: none;">\n')
		self.file.write('     <td style="border-collapse: collapse; border: none;">\n')
		self.file.write('       <a href="http://www.openairinterface.org/">\n')
		self.file.write('          <img src="http://www.openairinterface.org/wp-content/uploads/2016/03/cropped-oai_final_logo2.png" alt="" border="none" height=50 width=150>\n')
		self.file.write('          </img>\n')
		self.file.write('        </a>\n')
		self.file.write('      </td>\n')
		self.file.write('      <td style="border-collapse: collapse; border: none; vertical-align: center;">\n')
		self.file.write('        <b><font size = "6">Job Summary -- Job: ' + self.job_name + ' -- Build-ID: <a href="' + self.job_url + '">' + self.job_id + '</a></font></b>\n')
		self.file.write('      </td>\n')
		self.file.write('    </tr>\n')
		self.file.write('  </table>\n')
		self.file.write('  <br>\n')

		# Build Info Summary
		buildSummary = ''
		buildSummary += '  <table class="table-bordered" width = "80%" align = "center" border = "1">\n'
		buildSummary += '    <tr>\n'
		buildSummary += '      <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-time"></span> Build Start Time</td>\n'
		#date_formatted = re.sub('\..*', '', self.created)
		buildSummary += '      <td>' + self.job_start_time + '</td>\n'
		buildSummary += '    </tr>\n'
		buildSummary += '    <tr>\n'
		buildSummary += '      <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-wrench"></span> Build Trigger</td>\n'
		if self.git_pull_request:
			buildSummary += '      <td>Pull Request</td>\n'
		else:
			buildSummary += '      <td>Push Event</td>\n'
		buildSummary += '    </tr>\n'
		buildSummary += '    <tr>\n'
		buildSummary += '      <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-cloud-upload"></span> GIT Repository</td>\n'
		buildSummary += '      <td><a href="' + self.git_url + '">' + self.git_url + '</a></td>\n'
		buildSummary += '    </tr>\n'
		if self.git_pull_request:
			buildSummary += '	 <tr>\n'
			buildSummary += '      <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-log-out"></span> Merge Request URL</td>\n'
			buildSummary += '	   <td><a href="TEMPLATE_MERGE_REQUEST_LINK">TEMPLATE_MERGE_REQUEST_LINK</a></td>\n'
			buildSummary += '	 </tr>\n'
			buildSummary += '	 <tr>\n'
			buildSummary += '	   <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-header"></span> Merge Request Title</td>\n'
			buildSummary += '	   <td>TEMPLATE_MERGE_REQUEST_TEMPLATE</td>\n'
			buildSummary += '	 </tr>\n'
			buildSummary += '	 <tr>\n'
			buildSummary += '	   <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-log-out"></span> Source Branch</td>\n'
			buildSummary += '      <td>' + self.git_src_branch + '</td>\n'
			buildSummary += '    </tr>\n'
			buildSummary += '    <tr>\n'
			buildSummary += '      <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-tag"></span> Source Commit ID</td>\n'
			buildSummary += '      <td>' + self.git_src_commit + '</td>\n'
			buildSummary += '    </tr>\n'
			if (self.git_src_commit_msg is not None):
				buildSummary += '    <tr>\n'
				buildSummary += '      <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-comment"></span> Source Commit Message</td>\n'
				buildSummary += '      <td>' + self.git_src_commit_msg + '</td>\n'
				buildSummary += '    </tr>\n'
			buildSummary += '    <tr>\n'
			buildSummary += '      <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-log-in"></span> Target Branch</td>\n'
			buildSummary += '      <td>' + self.git_target_branch + '</td>\n'
			buildSummary += '    </tr>\n'
			buildSummary += '    <tr>\n'
			buildSummary += '      <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-tag"></span> Target Commit ID</td>\n'
			buildSummary += '      <td>' + self.git_target_commit + '</td>\n'
			buildSummary += '    </tr>\n'
		else:
			buildSummary += '    <tr>\n'
			buildSummary += '      <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-tree-deciduous"></span> Branch</td>\n'
			buildSummary += '      <td>' + self.git_src_branch + '</td>\n'
			buildSummary += '    </tr>\n'
			buildSummary += '    <tr>\n'
			buildSummary += '      <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-tag"></span> Commit ID</td>\n'
			buildSummary += '      <td>' + self.git_src_commit + '</td>\n'
			buildSummary += '    </tr>\n'
			if (self.git_src_commit_msg is not None):
				buildSummary += '    <tr>\n'
				buildSummary += '      <td bgcolor="lightcyan" > <span class="glyphicon glyphicon-comment"></span> Commit Message</td>\n'
				buildSummary += '      <td>' + self.git_src_commit_msg + '</td>\n'
				buildSummary += '    </tr>\n'
		buildSummary += '  </table>\n'
		buildSummary += '  <br>\n'
		self.file.write(buildSummary)

		cwd = os.getcwd()
		for reportFile in glob.glob('./*results_oai_*.html'):
			if reportFile == './test_results_oai_amf.html':
				continue
			newEpcReport = open(cwd + '/' + str(reportFile) + '.new', 'w')
			buildSummaryDone = True
			with open(cwd + '/' + str(reportFile), 'r') as originalEpcReport:
				for line in originalEpcReport:
					result = re.search('Deployment Summary', line)
					if (result is not None) and buildSummaryDone:
						newEpcReport.write(buildSummary)
						buildSummaryDone = False
					newEpcReport.write(line)
				originalEpcReport.close()
			newEpcReport.close()
			os.rename(cwd + '/' + str(reportFile) + '.new', cwd + '/' + str(reportFile))

	def generateFooter(self):
		self.file.write('  <div class="well well-lg">End of Build Report -- Copyright <span class="glyphicon glyphicon-copyright-mark"></span> 2020 <a href="http://www.openairinterface.org/">OpenAirInterface</a>. All Rights Reserved.</div>\n')
		self.file.write('</div></body>\n')
		self.file.write('</html>\n')

	def coding_formatting_log(self):
		cwd = os.getcwd()
		self.file.write('  <h2>OAI Coding / Formatting Guidelines Check</h2>\n')
		if os.path.isfile(cwd + '/src/oai_rules_result.txt'):
			cmd = 'grep NB_FILES_FAILING_CHECK ' + cwd + '/src/oai_rules_result.txt | sed -e "s#NB_FILES_FAILING_CHECK=##"'
			nb_fail = subprocess.check_output(cmd, shell=True, universal_newlines=True)
			cmd = 'grep NB_FILES_CHECKED ' + cwd + '/src/oai_rules_result.txt | sed -e "s#NB_FILES_CHECKED=##"'
			nb_total = subprocess.check_output(cmd, shell=True, universal_newlines=True)
			if int(nb_fail.strip()) == 0:
				self.file.write('  <div class="alert alert-success">\n')
				if self.git_pull_request:
					self.file.write('    <strong>All modified files in Pull-Request follow OAI rules. <span class="glyphicon glyphicon-ok-circle"></span> -> (' + nb_total.strip() + ' were checked)</strong>\n')
				else:
					self.file.write('    <strong>All files in repository follow OAI rules. <span class="glyphicon glyphicon-ok-circle"></span> -> (' + nb_total.strip() + ' were checked)</strong>\n')
				self.file.write('  </div>\n')
			else:
				self.file.write('  <div class="alert alert-danger">\n')
				if self.git_pull_request:
					self.file.write('    <strong>' + nb_fail.strip() + ' modified files in Pull-Request DO NOT follow OAI rules. <span class="glyphicon glyphicon-warning-sign"></span> -> (' + nb_total.strip() + ' were checked)</strong>\n')
				else:
					self.file.write('    <strong>' + nb_fail.strip() + ' files in repository DO NOT follow OAI rules. <span class="glyphicon glyphicon-warning-sign"></span> -> (' + nb_total.strip() + ' were checked)</strong>\n')
				self.file.write('  </div>\n')

				if os.path.isfile(cwd + '/src/oai_rules_result_list.txt'):
					self.file.write('  <button data-toggle="collapse" data-target="#oai-formatting-details">More details on formatting check</button>\n')
					self.file.write('  <div id="oai-formatting-details" class="collapse">\n')
					self.file.write('  <p>Please apply the following command to this(ese) file(s): </p>\n')
					self.file.write('  <p style="margin-left: 30px"><strong><code>cd src && clang-format -i filename(s)</code></strong></p>\n')
					self.file.write('  <table class="table-bordered" width = "60%" align = "center" border = 1>\n')
					self.file.write('    <tr><th bgcolor = "lightcyan" >Filename</th></tr>\n')
					with open(cwd + '/src/oai_rules_result_list.txt', 'r') as filelist:
						for line in filelist:
							self.file.write('    <tr><td>' + line.strip() + '</td></tr>\n')
						filelist.close()
					self.file.write('  </table>\n')
					self.file.write('  </div>\n')
		else:
			self.file.write('  <div class="alert alert-danger">\n')
			self.file.write('     <strong>Was NOT performed (with CLANG-FORMAT tool). <span class="glyphicon glyphicon-ban-circle"></span></strong>\n')
			self.file.write('  </div>\n')

		self.file.write('  <br>\n')

	def analyze_sca_log(self):
		cwd = os.getcwd()
		if os.path.isfile(cwd + '/archives/cppcheck_build.log'):
			self.file.write('  <h2>Static Code Analysis</h2>\n')
		if os.path.isfile(cwd + '/archives/cppcheck.xml'):
			nb_errors = 0
			nb_warnings = 0
			nb_uninitvar = 0
			nb_uninitStructMember = 0
			nb_memleak = 0
			nb_doubleFree = 0
			nb_resourceLeak = 0
			nb_nullPointer = 0
			nb_arrayIndexOutOfBounds = 0
			nb_bufferAccessOutOfBounds = 0
			nb_unknownEvaluationOrder = 0
			with open(cwd + '/archives/cppcheck.xml', 'r') as xmlfile:
				for line in xmlfile:
					result = re.search('severity="warning"', line)
					if result is not None:
						nb_warnings += 1
					result = re.search('severity="error"', line)
					if result is not None:
						nb_errors += 1
						result = re.search('uninitvar', line)
						if result is not None:
							nb_uninitvar += 1
						result = re.search('uninitStructMember', line)
						if result is not None:
							nb_uninitStructMember += 1
						result = re.search('memleak', line)
						if result is not None:
							nb_memleak += 1
						result = re.search('doubleFree', line)
						if result is not None:
							nb_doubleFree += 1
						result = re.search('resourceLeak', line)
						if result is not None:
							nb_resourceLeak += 1
						result = re.search('nullPointer', line)
						if result is not None:
							nb_nullPointer += 1
						result = re.search('arrayIndexOutOfBounds', line)
						if result is not None:
							nb_arrayIndexOutOfBounds += 1
						result = re.search('bufferAccessOutOfBounds', line)
						if result is not None:
							nb_bufferAccessOutOfBounds += 1
						result = re.search('unknownEvaluationOrder', line)
						if result is not None:
							nb_unknownEvaluationOrder += 1
				xmlfile.close()
			if (nb_errors == 0) and (nb_warnings == 0):
				self.file.write('   <div class="alert alert-success">\n')
				self.file.write('     <strong>CPPCHECK found NO error and NO warning <span class="glyphicon glyphicon-ok-circle"></span></strong>\n')
				self.file.write('   </div>\n')
			elif (nb_errors == 0):
				self.file.write('   <div class="alert alert-warning">\n')
				self.file.write('     <strong>CPPCHECK found NO error and ' + str(nb_warnings) + ' warnings <span class="glyphicon glyphicon-warning-sign"></span></strong>\n')
				self.file.write('   </div>\n')
			else:
				self.file.write('   <div class="alert alert-danger">\n')
				self.file.write('     <strong>CPPCHECK found ' +  str(nb_errors) + ' errors and ' + str(nb_warnings) + ' warnings <span class="glyphicon glyphicon-ban-circle"></span></strong>\n')
				self.file.write('   </div>\n')
			if (nb_errors > 0) or (nb_warnings > 0):
				self.file.write('   <button data-toggle="collapse" data-target="#oai-cppcheck-details">More details on CPPCHECK results</button>\n')
				self.file.write('   <div id="oai-cppcheck-details" class="collapse">\n')
				self.file.write('   <br>\n')
				self.file.write('   <table class="table-bordered" width = "80%" align = "center" border = "1">\n')
				self.file.write('     <tr bgcolor = "#33CCFF" >\n')
				self.file.write('   	<th>Error / Warning Type</th>\n')
				self.file.write('   	<th>Nb Errors</th>\n')
				self.file.write('   	<th>Nb Warnings</th>\n')
				self.file.write('     </tr>\n')
				self.file.write('     <tr>\n')
				self.file.write('   	<td>Uninitialized variable</td>\n')
				self.file.write('   	<td>' + str(nb_uninitvar) + '</td>\n')
				self.file.write('   	<td>N/A</td>\n')
				self.file.write('     </tr>\n')
				self.file.write('     <tr>\n')
				self.file.write('       <td>Uninitialized struct member</td>\n')
				self.file.write('       <td>' + str(nb_uninitStructMember) + '</td>\n')
				self.file.write('       <td>N/A</td>\n')
				self.file.write('     </tr>\n')
				self.file.write('     <tr>\n')
				self.file.write('       <td>Memory leak</td>\n')
				self.file.write('       <td>' + str(nb_memleak) + '</td>\n')
				self.file.write('       <td>N/A</td>\n')
				self.file.write('     </tr>\n')
				self.file.write('     <tr>\n')
				self.file.write('       <td>Memory is freed twice</td>\n')
				self.file.write('       <td>' + str(nb_doubleFree) + '</td>\n')
				self.file.write('       <td>N/A</td>\n')
				self.file.write('     </tr>\n')
				self.file.write('     <tr>\n')
				self.file.write('       <td>Resource leak</td>\n')
				self.file.write('       <td>' + str(nb_resourceLeak) + '</td>\n')
				self.file.write('       <td>N/A</td>\n')
				self.file.write('     </tr>\n')
				self.file.write('     <tr>\n')
				self.file.write('       <td>Possible null pointer dereference</td>\n')
				self.file.write('       <td>' + str(nb_nullPointer) + '</td>\n')
				self.file.write('       <td>N/A</td>\n')
				self.file.write('     </tr>\n')
				self.file.write('     <tr>\n')
				self.file.write('       <td>Array access  out of bounds</td>\n')
				self.file.write('       <td>' + str(nb_arrayIndexOutOfBounds) + '</td>\n')
				self.file.write('       <td>N/A</td>\n')
				self.file.write('     </tr>\n')
				self.file.write('     <tr>\n')
				self.file.write('   	<td>Buffer is accessed out of bounds</td>\n')
				self.file.write('   	<td>' + str(nb_bufferAccessOutOfBounds) + '</td>\n')
				self.file.write('   	<td>N/A</td>\n')
				self.file.write('     </tr>\n')
				self.file.write('     <tr>\n')
				self.file.write('       <td>Expression depends on order of evaluation of side effects</td>\n')
				self.file.write('       <td>' + str(nb_unknownEvaluationOrder) + '</td>\n')
				self.file.write('       <td>N/A</td>\n')
				self.file.write('     </tr>\n')
				self.file.write('     <tr>\n')
				self.file.write('       <td>Others</td>\n')
				nb_others = nb_uninitvar + nb_uninitStructMember + nb_memleak + nb_doubleFree + nb_resourceLeak + nb_nullPointer + nb_arrayIndexOutOfBounds + nb_arrayIndexOutOfBounds + nb_bufferAccessOutOfBounds + nb_unknownEvaluationOrder
				nb_others = nb_errors - nb_others
				self.file.write('   	<td>' + str(nb_others) + '</td>\n')
				self.file.write('   	<td>' + str(nb_warnings) + '</td>\n')
				self.file.write('     </tr>\n')
				self.file.write('     <tr bgcolor = "#33CCFF" >\n')
				self.file.write('   	<th>Total</th>\n')
				self.file.write('   	<th>' + str(nb_errors) + '</th>\n')
				self.file.write('   	<th>' + str(nb_warnings) + '</th>\n')
				self.file.write('     </tr>\n')
				self.file.write('   </table>\n')
				self.file.write('   <br>\n')
				self.file.write('   <p>Full details in artifact (cppcheck.xml) </p>\n')
				self.file.write('   <p style="margin-left: 30px">Graphical Interface tool : <strong><code>cppcheck-gui -l cppcheck.xml</code></strong></p>\n')
				self.file.write('   <br>\n')
				self.file.write('   </div>\n')
		else:
			self.file.write('  <div class="alert alert-danger">\n')
			self.file.write('     <strong>Was NOT performed (with CPPCHECK tool). <span class="glyphicon glyphicon-ban-circle"></span></strong>\n')
			self.file.write('  </div>\n')

	def buildSummaryHeader(self):
		self.file.write('  <h2>Docker / Podman Image Build Summary</h2>\n')
		self.file.write('  <table class="table-bordered" width = "100%" align = "center" border = "1">\n')
		self.file.write('     <tr bgcolor="#33CCFF" >\n')
		self.file.write('   	<th>Stage Name</th>\n')
		self.file.write('   	<th>Image Kind</th>\n')
		self.file.write('		<th>OAI AMF <font color="Gold">Ubuntu18</font> Image</th>\n')
		self.file.write('		<th>OAI AMF <font color="Gold">RHEL8</font> Image</th>\n')
		self.file.write('     </tr>\n')

	def buildSummaryFooter(self):
		self.file.write('  </table>\n')
		self.file.write('  <br>\n')
		if len(self.warning_rows) > 0:
			self.file.write('  <h3>Compilation Warnings Details</h3>\n')
			self.file.write('  <button data-toggle="collapse" data-target="#oai-compilation-details">Details for Compilation Errors and Warnings </button>\n')
			self.file.write('  <div id="oai-compilation-details" class="collapse">\n')
			self.file.write('  <table class="table-bordered">\n')
			self.file.write('    <tr bgcolor = "#33CCFF" >\n')
			self.file.write('      <th>File</th>\n')
			self.file.write('      <th>Line Number</th>\n')
			self.file.write('      <th>Status</th>\n')
			self.file.write('      <th>Message</th>\n')
			self.file.write(self.warning_rows)
			self.file.write('    </tr>\n')
			self.file.write('  </table>\n')
			self.file.write('  </div>\n')

	def initialGitSetup(self):
		self.file.write('    <tr>\n')
		self.file.write('      <td bgcolor="lightcyan" >Initial Git Setup</td>\n')
		self.analyze_docker_build_git_part('AMF')
		self.file.write('	 </tr>\n')

	def analyze_docker_build_git_part(self, nfType):
		if nfType != 'AMF':
			self.file.write('      <td>N/A</td>\n')
			self.file.write('      <td colspan="2">Wrong NF Type for this Report</td>\n')
			return

		self.file.write('      <td>Builder Image</td>\n')
		cwd = os.getcwd()
		variants = ['docker', 'podman']
		for variant in variants:
			logFileName = 'amf_' + variant + '_image_build.log'
			if os.path.isfile(cwd + '/archives/' + logFileName):
				status = False
				section_start_pattern = 'git config --global http'
				section_end_pattern = 'WORKDIR /openair-amf/build/scripts'
				section_status = False
				with open(cwd + '/archives/' + logFileName, 'r') as logfile:
					for line in logfile:
						result = re.search(section_start_pattern, line)
						if result is not None:
							section_status = True
						result = re.search(section_end_pattern, line)
						if result is not None:
							section_status = False
							status = True
					logfile.close()

				if status:
					cell_msg = '      <td bgcolor="LimeGreen"><pre style="border:none; background-color:LimeGreen"><b>'
					cell_msg += 'OK:\n'
					cell_msg += ' -- All Git Operations went successfully</b></pre></td>\n'
				else:
					cell_msg = '	  <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
					cell_msg += 'KO::\n'
					cell_msg += ' -- Some Git Operations went WRONG</b></pre></td>\n'
			else:
				cell_msg = '      <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
				cell_msg += 'KO: logfile (' + logFileName + ') not found</b></pre></td>\n'

			self.file.write(cell_msg)

	def installLibsPackagesRow(self):
		self.file.write('    <tr>\n')
		self.file.write('      <td bgcolor="lightcyan" >SW libs and packages Installation</td>\n')
		self.analyze_install_log('AMF')
		self.file.write('	 </tr>\n')

	def analyze_install_log(self, nfType):
		if nfType != 'AMF':
			self.file.write('      <td>N/A</td>\n')
			self.file.write('      <td colspan="2">Wrong NF Type for this Report</td>\n')
			return

		self.file.write('      <td>Builder Image</td>\n')

		cwd = os.getcwd()
		variants = ['docker', 'podman']
		for variant in variants:
			logFileName = 'amf_' + variant + '_image_build.log'
			if os.path.isfile(cwd + '/archives/' + logFileName):
				status = False
				section_start_pattern = 'build_amf --install-deps --force'
				section_end_pattern = 'build_amf --clean --Verbose --build-type Release --jobs'
				section_status = False
				package_install = False
				spdlog_build_start = False
				spdlog_build_status = False
				pistache_build_start = False
				pistache_build_status = False
				json_build_start = False
				json_build_status = False
				cpprest_build_start = False
				cpprest_build_status = False
				base_image = False
				with open(cwd + '/archives/' + logFileName, 'r') as logfile:
					for line in logfile:
						result = re.search('FROM oai-amf-base', line)
						if result is not None:
							base_image = True
						result = re.search(section_start_pattern, line)
						if result is not None:
							section_status = True
						result = re.search(section_end_pattern, line)
						if result is not None:
							section_status = True
						if section_status:
							result = re.search('AMF deps installation successful', line)
							if result is not None:
								status = True
							result = re.search('distro libs installation complete', line)
							if result is not None:
								package_install = True
							result = re.search('Starting to install cpprestsdk', line)
							if result is not None:
								cpprest_build_start = True
							result = re.search('cpprestsdk installation complete', line)
							if result is not None and cpprest_build_start:
								cpprest_build_status = True
							result = re.search('Starting to install spdlog', line)
							if result is not None:
								spdlog_build_start = True
							result = re.search('spdlog installation complete', line)
							if result is not None and spdlog_build_start:
								spdlog_build_status = True
							result = re.search('Starting to install pistache', line)
							if result is not None:
								pistache_build_start = True
							result = re.search('pistache installation complete', line)
							if result is not None and pistache_build_start:
								pistache_build_status = True
							result = re.search('Starting to install Nlohmann Json', line)
							if result is not None:
								json_build_status = True
							result = re.search('Nlohmann Json installation complete', line)
							if result is not None:
								json_build_status = True
					logfile.close()
				if base_image:
					cell_msg = '      <td bgcolor="LimeGreen"><pre style="border:none; background-color:LimeGreen"><b>'
					cell_msg += 'N/A:\n'
				elif status:
					cell_msg = '	  <td bgcolor="LimeGreen"><pre style="border:none; background-color:LimeGreen"><b>'
					cell_msg += 'OK:\n'
				else:
					cell_msg = '	  <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
					cell_msg += 'KO:\n'
				cell_msg += ' -- build_amf --install-deps --force\n'
				if base_image:
					cell_msg += '   ** Packages Installation: N/A\n'
				elif package_install:
					cell_msg += '   ** Packages Installation: OK\n'
				else:
					cell_msg += '   ** Packages Installation: KO\n'
				if base_image:
					cell_msg += '   ** cpprestsdk Installation: N/A\n'
				elif cpprest_build_status:
					cell_msg += '   ** cpprestsdk Installation: OK\n'
				else:
					cell_msg += '   ** cpprestsdk Installation: KO\n'
				if base_image:
					cell_msg += '   ** spdlog Installation: N/A\n'
				elif spdlog_build_status:
					cell_msg += '   ** spdlog Installation: OK\n'
				else:
					cell_msg += '   ** spdlog Installation: KO\n'
				if base_image:
					cell_msg += '   ** pistache Installation: N/A\n'
				elif pistache_build_status:
					cell_msg += '   ** pistache Installation: OK\n'
				else:
					cell_msg += '   ** pistache Installation: KO\n'
				if base_image:
					cell_msg += '   ** Nlohmann Json Installation: N/A\n'
				elif json_build_status:
					cell_msg += '   ** Nlohmann Json Installation: OK\n'
				else:
					cell_msg += '   ** Nlohmann Json Installation: KO\n'
				cell_msg += '</b></pre></td>\n'
			else:
				cell_msg = '	  <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
				cell_msg += 'KO: logfile (' + logFileName + ') not found</b></pre></td>\n'

			self.file.write(cell_msg)

	def buildCompileRows(self):
		self.file.write('	 <tr>\n')
		self.file.write('	   <td rowspan=2 bgcolor="lightcyan" >cNF Compile / Build</td>\n')
		self.analyze_build_log('AMF', True)
		self.file.write('	 </tr>\n')
		self.file.write('	 <tr>\n')
		self.analyze_compile_log('AMF', True)
		self.file.write('	 </tr>\n')

	def analyze_build_log(self, nfType, imageKind):
		if nfType != 'AMF':
			if imageKind:
				self.file.write('      <td>N/A</td>\n')
			self.file.write('	   <td colspan="2">Wrong NF Type for this Report</td>\n')
			return

		if imageKind:
			self.file.write('      <td>Builder Image</td>\n')

		cwd = os.getcwd()
		variants = ['docker', 'podman']
		for variant in variants:
			logFileName = 'amf_' + variant + '_image_build.log'
			if os.path.isfile(cwd + '/archives/' + logFileName):
				status = False
				if nfType == 'AMF':
					section_start_pattern = 'build_amf --clean --Verbose --build-type Release --jobs'
					section_end_pattern = 'FROM .* as oai-amf$'
					pass_pattern = 'amf installed'
				section_status = False
				with open(cwd + '/archives/' + logFileName, 'r') as logfile:
					for line in logfile:
						result = re.search(section_start_pattern, line)
						if result is not None:
							section_status = True
						result = re.search(section_end_pattern, line)
						if result is not None:
							section_status = False
						if section_status:
							result = re.search(pass_pattern, line)
							if result is not None:
								status = True
					logfile.close()
				if status:
					cell_msg = '	  <td bgcolor="LimeGreen"><pre style="border:none; background-color:LimeGreen"><b>'
					cell_msg += 'OK:\n'
				else:
					cell_msg = '	  <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
					cell_msg += 'KO:\n'
				cell_msg += ' -- ' + section_start_pattern + '</b></pre></td>\n'
			else:
				cell_msg = '	  <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
				cell_msg += 'KO: logfile (' + logFileName + ') not found</b></pre></td>\n'

			self.file.write(cell_msg)

	def analyze_compile_log(self, nfType, imageKind):
		if nfType != 'AMF':
			if imageKind:
				self.file.write('      <td>N/A</td>\n')
			self.file.write('	   <td>Wrong NF Type for this Report</td>\n')
			return

		if imageKind:
			self.file.write('      <td>Builder Image</td>\n')

		cwd = os.getcwd()
		variants = ['docker', 'podman']
		for variant in variants:
			logFileName = 'amf_' + variant + '_image_build.log'
			nb_errors = 0
			nb_warnings = 0
			added_separator = False

			if os.path.isfile(cwd + '/archives/' + logFileName):
				if nfType == 'AMF':
					section_start_pattern = 'build_amf --clean --Verbose --build-type Release --jobs'
					section_end_pattern = 'FROM .* as oai-amf$'
				section_status = False
				with open(cwd + '/archives/' + logFileName, 'r') as logfile:
					for line in logfile:
						result = re.search(section_start_pattern, line)
						if result is not None:
							section_status = True
						result = re.search(section_end_pattern, line)
						if result is not None:
							section_status = False
						if section_status:
							result = re.search('error:', line)
							if result is not None:
								nb_errors += 1
							result = re.search('warning:', line)
							if result is not None:
								nb_warnings += 1
							result = re.search('error:|warning:', line)
							if result is not None:
								correctLine = re.sub("^.*/openair-amf","/openair-amf",line.strip())
								wordsList = correctLine.split(None,2)
								filename = re.sub(":[0-9]*:[0-9]*:","", wordsList[0])
								linenumber = re.sub(filename + ':',"", wordsList[0])
								linenumber = re.sub(':[0-9]*:',"", linenumber)
								error_warning_status = re.sub(':',"", wordsList[1])
								error_warning_msg = re.sub('^.*' + error_warning_status + ':', '', correctLine)

								if not added_separator and variant == 'docker':
									self.warning_rows += '<tr><td colspan="4" align = "center" bgcolor = "LightGray"><b>Ubuntu 18</b></td></tr>\n'
									added_separator = True
								if not added_separator and variant == 'podman':
									self.warning_rows += '<tr><td colspan="4" align = "center" bgcolor = "LightGray"><b>RHEL 8</b></td></tr>\n'
									added_separator = True
								self.warning_rows += '<tr><td>' + filename + '</td><td>' + linenumber + '</td><td>' + error_warning_status + '</td><td>' + error_warning_msg + '</td></tr>\n'
					logfile.close()
				if nb_warnings == 0 and nb_errors == 0:
					cell_msg = '	   <td bgcolor="LimeGreen"><pre style="border:none; background-color:LimeGreen"><b>'
				elif nb_warnings < 20 and nb_errors == 0:
					cell_msg = '	   <td bgcolor="Orange"><pre style="border:none; background-color:Orange"><b>'
				else:
					cell_msg = '	   <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
				if nb_errors > 0:
					cell_msg += str(nb_errors) + ' errors found in compile log\n'
				cell_msg += str(nb_warnings) + ' warnings found in compile log</b></pre></td>\n'
				self.nb_warnings = nb_warnings
			else:
				cell_msg = '	  <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
				cell_msg += 'KO: logfile (' + logFileName + ') not found</b></pre></td>\n'

			self.file.write(cell_msg)

	def copyToTargetImage(self):
		self.file.write('	 <tr>\n')
		self.file.write('	   <td bgcolor="lightcyan" >SW libs Installation / Copy from Builder</td>\n')
		self.analyze_copy_log('AMF')
		self.file.write('	 </tr>\n')

	def analyze_copy_log(self, nfType):
		if nfType != 'AMF':
			self.file.write('      <td>N/A</td>\n')
			self.file.write('	   <td colspan="2">Wrong NF Type for this Report</td>\n')
			return

		self.file.write('      <td>Target Image</td>\n')

		cwd = os.getcwd()
		variants = ['docker', 'podman']
		for variant in variants:
			logFileName = 'amf_' + variant + '_image_build.log'
			if os.path.isfile(cwd + '/archives/' + logFileName):
				section_start_pattern = 'FROM .* as oai-amf$'
				section_end_pattern = 'WORKDIR /openair-amf/etc'
				section_status = False
				status = False
				with open(cwd + '/archives/' + logFileName, 'r') as logfile:
					for line in logfile:
						result = re.search(section_start_pattern, line)
						if result is not None:
							section_status = True
						result = re.search(section_end_pattern, line)
						if result is not None:
							section_status = False
							status = True
					logfile.close()
				if status:
					cell_msg = '	   <td bgcolor="LimeGreen"><pre style="border:none; background-color:LimeGreen"><b>'
					cell_msg += 'OK:\n'
				else:
					cell_msg = '	   <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
					cell_msg += 'KO:\n'
				cell_msg += '</b></pre></td>\n'
			else:
				cell_msg = '	  <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
				cell_msg += 'KO: logfile (' + logFileName + ') not found</b></pre></td>\n'

			self.file.write(cell_msg)

	def copyConfToolsToTargetImage(self):
		self.file.write('	 <tr>\n')
		self.file.write('	   <td bgcolor="lightcyan" >Copy Template Conf / Tools from Builder</td>\n')
		self.analyze_copy_conf_tool_log('AMF')
		self.file.write('	 </tr>\n')

	def analyze_copy_conf_tool_log(self, nfType):
		if nfType != 'AMF':
			self.file.write('      <td>N/A</td>\n')
			self.file.write('	   <td colspan="2">Wrong NF Type for this Report</td>\n')
			return

		self.file.write('      <td>Target Image</td>\n')

		cwd = os.getcwd()
		variants = ['docker', 'podman']
		for variant in variants:
			logFileName = 'amf_' + variant + '_image_build.log'
			if os.path.isfile(cwd + '/archives/' + logFileName):
				section_start_pattern = 'WORKDIR /openair-amf/etc'
				if variant == 'docker':
					section_end_pattern = 'Successfully tagged oai-amf'
				else:
					section_end_pattern = 'COMMIT oai-amf:' 
				section_status = False
				status = False
				with open(cwd + '/archives/' + logFileName, 'r') as logfile:
					for line in logfile:
						result = re.search(section_start_pattern, line)
						if result is not None:
							section_status = True
						result = re.search(section_end_pattern, line)
						if result is not None:
							section_status = False
							status = True
					logfile.close()
				if status:
					cell_msg = '	   <td bgcolor="LimeGreen"><pre style="border:none; background-color:LimeGreen"><b>'
					cell_msg += 'OK:\n'
				else:
					cell_msg = '	   <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
					cell_msg += 'KO:\n'
				cell_msg += '</b></pre></td>\n'
			else:
				cell_msg = '	  <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
				cell_msg += 'KO: logfile (' + logFileName + ') not found</b></pre></td>\n'

			self.file.write(cell_msg)

	def imageSizeRow(self):
		self.file.write('	 <tr>\n')
		self.file.write('	   <td bgcolor="lightcyan" >Image Size</td>\n')
		self.analyze_image_size_log('AMF', True)
		self.file.write('	 </tr>\n')

	def analyze_image_size_log(self, nfType, imageKind):
		if nfType != 'AMF':
			if imageKind:
				self.file.write('      <td>N/A</td>\n')
			self.file.write('	   <td colspan="2">Wrong NF Type for this Report</td>\n')
			return

		if imageKind:
			self.file.write('      <td>Target Image</td>\n')

		cwd = os.getcwd()
		variants = ['docker', 'podman']
		for variant in variants:
			logFileName = 'amf_' + variant + '_image_build.log'
			if os.path.isfile(cwd + '/archives/' + logFileName):
				if nfType == 'AMF':
					if variant == 'docker':
						section_start_pattern = 'Successfully tagged oai-amf'
						section_end_pattern = 'OAI-AMF DOCKER IMAGE BUILD'
					else:
						section_start_pattern = 'COMMIT oai-amf:'
						section_end_pattern = 'OAI-AMF PODMAN RHEL8 IMAGE BUILD'
				section_status = False
				status = False
				with open(cwd + '/archives/' + logFileName, 'r') as logfile:
					for line in logfile:
						result = re.search(section_start_pattern, line)
						if result is not None:
							section_status = True
						result = re.search(section_end_pattern, line)
						if result is not None:
							section_status = True
						if section_status:
							if nfType == 'AMF':
								if self.git_pull_request:
									result = re.search('oai-amf *ci-tmp', line)
								else:
									result = re.search('oai-amf *develop', line)
							if result is not None and not status:
								result = re.search('ago  *([0-9A-Z ]+)', line)
								if result is not None:
									size = result.group(1)
									if variant == 'docker':
										size = re.sub('MB', ' MB', size)
									status = True
					logfile.close()
				if status:
					cell_msg = '	   <td bgcolor="LimeGreen"><pre style="border:none; background-color:LimeGreen"><b>'
					cell_msg += 'OK:  ' + size + '\n'
				else:
					cell_msg = '	   <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
					cell_msg += 'KO:\n'
				cell_msg += '</b></pre></td>\n'
			else:
				cell_msg = '	  <td bgcolor="Tomato"><pre style="border:none; background-color:Tomato"><b>'
				cell_msg += 'KO: logfile (' + logFileName + ') not found</b></pre></td>\n'

			self.file.write(cell_msg)

def Usage():
	print('----------------------------------------------------------------------------------------------------------------------')
	print('generateHtmlReport.py')
	print('   Generate an HTML report for the Jenkins pipeline on openair-amf.')
	print('----------------------------------------------------------------------------------------------------------------------')
	print('Usage: python3 generateHtmlReport.py [options]')
	print('  --help  Show this help.')
	print('---------------------------------------------------------------------------------------------- Mandatory Options -----')
	print('  --job_name=[Jenkins Job name]')
	print('  --job_id=[Jenkins Job Build ID]')
	print('  --job_url=[Jenkins Job Build URL]')
	print('  --git_url=[Git Repository URL]')
	print('  --git_src_branch=[Git Source Branch Name]')
	print('  --git_src_commit=[Git Source Commit SHA-ONE]')
	print('----------------------------------------------------------------------------------------------- Optional Options -----')
	print('  --git_pull_request=True')
	print('  --git_target_branch=[Git Target Branch Name]')
	print('  --git_target_commit=[Git Target Commit SHA-ONE]')

#--------------------------------------------------------------------------------------------------------
#
# Start of main
#
#--------------------------------------------------------------------------------------------------------

argvs = sys.argv
argc = len(argvs)

HTML = HtmlReport()

while len(argvs) > 1:
	myArgv = argvs.pop(1)
	if re.match('^\-\-help$', myArgv, re.IGNORECASE):
		Usage()
		sys.exit(0)
	elif re.match('^\-\-job_name=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-job_name=(.+)$', myArgv, re.IGNORECASE)
		HTML.job_name = matchReg.group(1)
	elif re.match('^\-\-job_id=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-job_id=(.+)$', myArgv, re.IGNORECASE)
		HTML.job_id = matchReg.group(1)
	elif re.match('^\-\-job_url=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-job_url=(.+)$', myArgv, re.IGNORECASE)
		HTML.job_url = matchReg.group(1)
	elif re.match('^\-\-git_url=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-git_url=(.+)$', myArgv, re.IGNORECASE)
		HTML.git_url = matchReg.group(1)
	elif re.match('^\-\-git_src_branch=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-git_src_branch=(.+)$', myArgv, re.IGNORECASE)
		HTML.git_src_branch = matchReg.group(1)
	elif re.match('^\-\-git_src_commit=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-git_src_commit=(.+)$', myArgv, re.IGNORECASE)
		HTML.git_src_commit = matchReg.group(1)
	elif re.match('^\-\-git_src_commit_msg=(.+)$', myArgv, re.IGNORECASE):
		# Not Mandatory
		matchReg = re.match('^\-\-git_src_commit_msg=(.+)$', myArgv, re.IGNORECASE)
		HTML.git_src_commit_msg = matchReg.group(1)
	elif re.match('^\-\-git_pull_request=(.+)$', myArgv, re.IGNORECASE):
		# Can be silent: would be false!
		matchReg = re.match('^\-\-git_pull_request=(.+)$', myArgv, re.IGNORECASE)
		if matchReg.group(1) == 'true' or matchReg.group(1) == 'True':
			HTML.git_pull_request = True
	elif re.match('^\-\-git_target_branch=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-git_target_branch=(.+)$', myArgv, re.IGNORECASE)
		HTML.git_target_branch = matchReg.group(1)
	elif re.match('^\-\-git_target_commit=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-git_target_commit=(.+)$', myArgv, re.IGNORECASE)
		HTML.git_target_commit = matchReg.group(1)
	else:
		sys.exit('Invalid Parameter: ' + myArgv)

if HTML.job_name == '' or HTML.job_id == '' or HTML.job_url == '':
	sys.exit('Missing Parameter in job description')

if HTML.git_url == '' or HTML.git_src_branch == '' or HTML.git_src_commit == '':
	sys.exit('Missing Parameter in Git Repository description')

if HTML.git_pull_request:
	if HTML.git_target_commit == '' or HTML.git_target_branch == '':
		 sys.exit('Missing Parameter in Git Pull Request Repository description')

HTML.generate()
