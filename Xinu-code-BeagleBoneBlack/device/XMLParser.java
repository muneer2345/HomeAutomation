import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

class Thing {
	
	String name;
	int gpio_pin;
	String mode;
	String func1;
	int id;
	String func2;

	public int getId() {
		return id;
	}
	public String getFunc1() {
		return func1;
	}
	public void setFunc1(String func1) {
		this.func1 = func1;
	}
	public String getFunc2() {
		return func2;
	}
	public void setFunc2(String func2) {
		this.func2 = func2;
	}
	
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getGpio_pin() {
		return gpio_pin;
	}
	public void setGpio_pin(int gpio_pin) {
		this.gpio_pin = gpio_pin;
	}
	public String getMode() {
		return mode;
	}
	public void setMode(String mode) {
		this.mode = mode;
	}
	public void setId(int id) {
		this.id = id;
	}
	
}

public class XMLParser {

	public static void main(String argv[]) throws IOException {
		
		List<Thing> ThingList = new ArrayList<Thing>();

		
		/* Parsing the XML part and adding the details into thing list */
		try {

			File xmlfile = new File("DDLfile.xml");
			DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			Document doc = dBuilder.parse(xmlfile);

			doc.getDocumentElement().normalize();

			System.out.println("Root Element is :" + doc.getDocumentElement().getNodeName());

			NodeList nList = doc.getElementsByTagName("device");

			System.out.println("----------------------------");

			for (int temp = 0; temp < nList.getLength(); temp++) {

				Node nNode = nList.item(temp);

				System.out.println("\nCurrent Element :" + nNode.getNodeName());
				
				Thing t1 = new Thing(); 

				if (nNode.getNodeType() == Node.ELEMENT_NODE) {

					Element eElement = (Element) nNode;
					
					
					
					System.out.println("Name : " + eElement.getElementsByTagName("name").item(0).getTextContent());
					t1.setName(eElement.getElementsByTagName("name").item(0).getTextContent());
					
					System.out.println("ID : " + eElement.getElementsByTagName("id").item(0).getTextContent());
					t1.setId(Integer.parseInt(eElement.getElementsByTagName("id").item(0).getTextContent()));
					
					
					System.out.println("Mode : " + eElement.getElementsByTagName("mode").item(0).getTextContent());
					t1.setMode(eElement.getElementsByTagName("mode").item(0).getTextContent());
					
					//System.out.println("Default_state : " + eElement.getElementsByTagName("default_state").item(0).getTextContent());
					//System.out.println("gpio_pin : " + eElement.getElementsByTagName("gpio_pin").item(0).getTextContent());
					//t1.setGpio_pin(Integer.parseInt(eElement.getElementsByTagName("gpio_pin").item(0).getTextContent()));
					
					t1.setFunc1(eElement.getElementsByTagName("func1").item(0).getTextContent());
					
					t1.setFunc2(eElement.getElementsByTagName("func2").item(0).getTextContent());
				}
				
				ThingList.add(t1);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		/* Code generation part */
		
		/* generate the source code to be added based on thing contents*/
		
		StringBuilder SB = new StringBuilder();
		StringBuilder SB2 = new StringBuilder();	

		for(Thing t1:ThingList) {
		if(t1.getMode().equals("out")) {

			System.out.println("Iterating over Thing");
			SB.append("case " + t1.getId() + " : \n"); 
			SB.append(t1.getFunc1() + "(dev_name,  buf, val );\n" );
			SB.append("break; \n");
		} else if(t1.getMode().equals("in")) {

			System.out.println("Iterating over Thing");
			SB2.append("case " + t1.getId() + " : \n"); 
			SB2.append(t1.getFunc2() + "(dev_name,  buf, val );\n" ); 
			SB2.append("break; \n");
			}
		}
		
		SB.deleteCharAt(SB.length()-1);
		SB2.deleteCharAt(SB2.length()-1);

		/* Read and modify the source file and write that to a output file - 
		 * This is the file which will be used, */
		
		
		try {
			
			FileReader sourcefile;
			
			sourcefile = new FileReader("Source_file.txt");
			
			BufferedReader br = new BufferedReader(sourcefile);
			
			BufferedWriter bw = new BufferedWriter(new FileWriter("../system/driver.c"));
			
			StringBuilder tempstr = new StringBuilder();
			
			String line;
			
			while((line = br.readLine()) != null) {
				if (line.contains("#Add_out_code_here")) {
					String str = line.replace("#Add_out_code_here", "");
					tempstr.append(str);
					tempstr.append(SB.toString().replaceAll("\n",'\n'+str));
					tempstr.append("\n");
				} else if (line.contains("#Add_in_code_here")) {
					String str = line.replace("#Add_in_code_here", "");
					tempstr.append(str);
					tempstr.append(SB2.toString().replaceAll("\n",'\n'+str));
					tempstr.append("\n");
				} else {
					tempstr.append(line).append("\n");
				}
				
				
			}
			
			bw.write(tempstr.toString()) ;
			
			br.close();
			bw.close();
			
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		
	}



}
