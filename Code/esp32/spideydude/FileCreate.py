'''
  Make sure the html file is in the same directory
  having name "index.htm". Run this code. It will create
  a header file "spideyserver.h". Use that in the program
'''

#Reading the index.htm file contents
def getFileContent():
    file = open("index.htm")
    text = file.read().replace("\n"," ").replace("  ","")
    file.close()
    return text

# Processing the index.htm file contents
def ProcessContent(text):
    text = text.split("<style>",1)
    start = text[0]
    text = "<style>"+text[1]
    text = text.split("</head>",1)          
    style = text[0]+"</head>"
    text = text[1]
    text = text.split("<script>",1)
    body = text[0]
    text = "<script>" + text[1]
    text = text.split("</script>",1)
    script = text[0]+"</script>"
    end = text[1]
    return start,style,body,script,end

# Create the the content for the spideyserver.h lib file
def createContent(start,style,body,script,end):
    cont = "#ifndef __SPIDYDUDE_SERVER_H__\n#define __SPIDYDUDE_SERVER_H__\n\n"
    cont = cont +"String start = \""+ start.replace("\"","\\\"") + "\";\n"
    cont = cont +"String style = \""+ style.replace("\"","\\\"") + "\";\n"
    cont = cont +"String body = \""+ body.replace("\"","\\\"") + "\";\n"
    cont = cont +"String script = \""+ script.replace("\"","\\\"") + "\";\n"
    cont = cont +"String end = \""+ end.replace("\"","\\\"") + "\";\n"
    cont = cont + "String root(){ return start+style+body+script+end;}\n"
    cont = cont + "#endif\n"
    return cont

# Write the data on the file 
def writeFile(cont):
    f = open("spideyserver.h", "w")
    f.write(cont)
    f.close()

# Main function. Execution starts here
if __name__=="__main__":
    print("Reading index.htm file...")
    htmlFileContent = getFileContent()
    print("Processing the contents of the file...")
    start,style,body,script,end = ProcessContent(htmlFileContent)
    print("Creating the content for header file...")
    headerFileContent = createContent(start,style,body,script,end)
    print("File spideyserver.h created.")
    
