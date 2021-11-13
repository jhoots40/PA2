package main;
import java.io.*;
import java.util.*;

public class Disassembler {

	List<String> instructionBinary = new ArrayList<String>();
	List<String> instructionAssembly = new ArrayList<String>();
	
	
	//turns instructionBinary strings into instructionAssembly strings
	//most of the work
	void handleBinary()
	{
		return;
	}
	
	//prints the instructions to the console
	void printInstructions()
	{
		for(int i = 0; i < instructionAssembly.size(); i++)
		{
			System.out.println(instructionAssembly.get(i));
		}
	}
	
	//reads the binary instructions from file
	void readFile(String path) {
		try 
		{
			InputStream inputStream = new FileInputStream(path);
			while(inputStream.available() != 0)
			{
				byte[] bytes = new byte[4];
				inputStream.read(bytes);
				String toAdd = Base64.getEncoder().encodeToString(bytes);
				instructionBinary.add(toAdd);
			}
			inputStream.close();
		} 
		catch (IOException e)
		{
			e.printStackTrace();
		}
		
		return;
	}
	
	public static void main(String args[])
	{
		System.out.println("Hello World");
		Disassembler d = new Disassembler();
		//d.readFile("Path of file");
		d.handleBinary();
		d.printInstructions();
	}
}

