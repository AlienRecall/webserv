import sys

def create_html(input_file, output_file):
    with open(input_file, 'r') as f:
        post_data = f.read()

    # Crea il contenuto HTML
    html_content = f"""
    <html>
    <head>
        <title>Response</title>
    </head>
    <body>
        <h1>Data Received</h1>
        <p>{post_data}</p>
    </body>
    </html>
    """

    with open(output_file, 'w') as f:
        f.write(html_content)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 create_html.py input_file output_file")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]
    create_html(input_file, output_file)
