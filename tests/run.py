import json, tempfile, pathlib, subprocess

ENSURE_KEYS = ["blockIds", "causeFound", "line", "messageId", "stderr", "text", "vars"]

## Tools funtions

COLORS = {
	'red' : '\u001b[31m',
	'green' : '\u001b[32m',
	'reset' : '\u001b[0m',
	'yellow' : '\u001b[33m'
}
PYFE_PATH = pathlib.Path(__file__).parent.parent.absolute() / 'pyfe'



class InvalidTestDatas(Exception):
	pass

class TestError(Exception):
	pass

def get_all_py(p, l):
	if p.is_file():
		if p.name.endswith('.py'):
			l.append(p)
	else:
		for p2 in p.iterdir():
			get_all_py(p2, l)

## Tests

def simple_tests(cur_path):
	tests_dir = cur_path / 'simple_tests'
	tests = []
	get_all_py(tests_dir, tests)
	tests.sort()

	for py_path in tests:
		print("===============", str(py_path.relative_to(cur_path)))
		prog_lang = "python"
		if "blockly" in str(py_path):
			prog_lang = "blockly"
		if "scratch" in str(py_path):
			prog_lang = "scratch"

		json_path = py_path.with_suffix('.json')
		if not json_path.exists():
			raise InvalidTestDatas(str(json_path) + ' does not exists')
		try:
			with open(str(json_path)) as f:
				json_valid = json.load(f)
		except json.decoder.JSONDecodeError as e:
			print(COLORS['red']+"Invalid json file:", str(json_path), COLORS['reset'])
			raise e

		with tempfile.NamedTemporaryFile() as stdout, tempfile.NamedTemporaryFile() as stderr, tempfile.NamedTemporaryFile() as json_out_file:
			subprocess.run(["python3", str(py_path)], stdout=stdout, stderr=stderr)
			r = subprocess.run([str(PYFE_PATH), str(py_path), stderr.name, json_out_file.name, prog_lang], stdout=stdout)

			if r.returncode != 0:
				raise TestError("Return code = "+str(r.returncode))
			try:
				json_output = json.load(json_out_file)
			except json.decoder.JSONDecodeError as e:
				raise TestError("Invalid json output format")

		for key in ENSURE_KEYS:
			if key not in json_output:
				raise TestError("Json output, key {} not found".format(key))
		for key, val in json_valid.items():
			if json_output[key] != val:
				raise TestError("Json output, key '{}' : '{}' instead of '{}'".format(key, json_output[key], val))

		if bool(json_output['messageId']) != json_output['causeFound']:
			raise TestError("Invalid 'causeFound' value")
		
		if json_output['causeFound'] and not json_output['text']:
			raise TestError("Text is empty")

		print(COLORS['green'] + 'OK' + COLORS['reset'])
		print(COLORS['yellow'] + json_output['text'] + COLORS['reset'])


def main():
	cur_path = pathlib.Path(__file__).parent.absolute()
	
	try:
		simple_tests(cur_path)
	except TestError as e:
		print(COLORS['red'] + 'ERROR:', *e.args, COLORS['reset'])

if __name__ == '__main__':
	main()