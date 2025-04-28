async function sendRequest(method) {
  let url = document.getElementById(method.toLowerCase() + 'Url').value;
  let data = document.getElementById(method.toLowerCase() + 'Data')?.value || null;
  let options = { method };
  if (data && (method === 'POST')) {
    options.headers = { 'Content-Type': 'application/json' };
    options.body = JSON.stringify(JSON.parse(data));
  }
  try {
    let response = await fetch(url, options);
    let result = await response.text();
    document.getElementById(method.toLowerCase() + 'Response').innerText = result;
  } catch (error) {
    document.getElementById(method.toLowerCase() + 'Response').innerText = 'Error: ' + error;
  }
}
