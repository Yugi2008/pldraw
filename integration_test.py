import pexpect
import pexpect.replwrap as replwrap
import unittest
import os
	
# the postlisp executable
cmd = './postlisp'

# the prompt to expect
prompt = u'postlisp>'

class TestREPL(unittest.TestCase):

	def setUp(self):
		self.wrapper = replwrap.REPLWrapper(cmd, prompt, None)
		
	def test_add(self):
		output = self.wrapper.run_command(u'(1 2 +)')
		self.assertEqual(output.strip(), "(3)")
		output = self.wrapper.run_command(u'(1 2 10 +)')
		self.assertEqual(output.strip(), "(13)")
		output = self.wrapper.run_command(u'(1 2 0 +)')
		self.assertEqual(output.strip(), "(3)")
		output = self.wrapper.run_command(u'(0 1 2 +)')
		self.assertEqual(output.strip(), "(3)")
		output = self.wrapper.run_command(u'(1 2 -2 +)')
		self.assertEqual(output.strip(), "(1)")
		output = self.wrapper.run_command(u'(-1 -2 +)')
		self.assertEqual(output.strip(), "(-3)")

	def test_define(self):
		output = self.wrapper.run_command(u'(a True define)')
		self.assertEqual(output.strip(), "(True)")
		output = self.wrapper.run_command(u'(b a define)')
		self.assertEqual(output.strip(), "(True)")
		output = self.wrapper.run_command(u'(a b and)')
		self.assertEqual(output.strip(), "(True)")

	def test_conditional(self):
		self.wrapper.run_command(u'(a -1 define)')
		self.wrapper.run_command(u'(b 1 define)')
		self.wrapper.run_command(u'(c 0 define)')
		output = self.wrapper.run_command(u'((a b <) c False if)')
		self.assertEqual(output.strip(), "(0)")

	def test_error(self):
		output = self.wrapper.run_command(u'(begin True define)')
		self.assertTrue(output.strip().startswith('Error'))
				
class TestExecuteCommandline(unittest.TestCase):
		
	def test_sub(self):
		args = ' -e ' + ' "(4 2 -)" '
		(output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
		self.assertEqual(retcode, 0)
		self.assertEqual(output.strip(), b"(2)")

	def test_parse_error(self):
		args = ' -e ' + ' "(1 2 +) (3 4 +)" '
		(output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
		self.assertNotEqual(retcode, 0)
		self.assertTrue(output.strip().startswith(b'Error'))
		
	def test_error(self):
		args = ' -e ' + ' "(4 2 12 -)" '
		(output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
		self.assertNotEqual(retcode, 0)
		self.assertTrue(output.strip().startswith(b'Error'))

class TestExecuteFromFile(unittest.TestCase):
		
	def test_file(self):
		args = ' /mnt/tests/test3.slp'
		(output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
		self.assertEqual(retcode, 0)
		self.assertEqual(output.strip(), b"(2)")

	def test_file_lf(self):
		args = ' /mnt/tests/test4.slp'
		(output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
		self.assertEqual(retcode, 0)
		self.assertEqual(output.strip(), b"(-1)")

	def test_file_crlf(self):
		args = ' /mnt/tests/test_crlf.slp'
		(output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
		self.assertEqual(retcode, 0)
		self.assertEqual(output.strip(), b"(-1)")

	def test_file_error(self):
		args = ' /there/is/no/such/file'
		(output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
		self.assertNotEqual(retcode, 0)
		self.assertTrue(output.strip().startswith(b'Error'))

# run the tests
unittest.main()
