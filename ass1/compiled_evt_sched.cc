import java.util.*;
import java.io.*;

class State{
	public int numberInQueue = 0, accumulated = 0, noMeasurements = 0;
	

	Random slump = new Random();
	SimpleFileWriter W = new SimpleFileWriter("number.m", false);
	
	public void TreatEvent(Event x){
		switch (x.eventType){
			case G.ARRIVAL:{
				numberInQueue++;
				EventList.InsertEvent(G.ARRIVAL, G.time - (1/0.9)*Math.log(slump.nextDouble()));
				if (numberInQueue == 1){
					EventList.InsertEvent(G.READY, G.time + 1);
				}
			} break;
			case G.READY:{
				numberInQueue--;
				if (numberInQueue > 0){
					EventList.InsertEvent(G.READY, G.time + 1);
				}
			} break;
			case G.MEASURE:{
				accumulated = accumulated + numberInQueue;
				noMeasurements++;
				EventList.InsertEvent(G.MEASURE, G.time + 5);
				W.println(String.valueOf(numberInQueue));
			} break;
		}
	}
}
class Event{
	public double eventTime;
	public int eventType;
	public Event next;
}

public class EventList{
	
	public static Event list, last;
	
	EventList(){
		EventList.list = new Event();
    	EventList.last = new Event();
    	EventList.list.next = EventList.last;
	}
	
	public static void InsertEvent(int type, double TimeOfEvent){
 	Event dummy, predummy;
 	Event newEvent = new Event();
 	newEvent.eventType = type;
 	newEvent.eventTime = TimeOfEvent;
 	predummy = list;
 	dummy = list.next;
 	while ((dummy.eventTime < newEvent.eventTime) & (dummy != last)){
 		predummy = dummy;
 		dummy = dummy.next;
 	}
 	predummy.next = newEvent;
 	newEvent.next = dummy;
 }
	
	public static Event FetchEvent(){
		Event dummy;
		dummy = list.next;
		list.next = dummy.next;
		dummy.next = null;
		return dummy;
	}
}

public class G{
	public static final int ARRIVAL = 1, READY = 2, MEASURE = 3;
	public static double time = 0;
}

import java.util.*;
import java.io.*;


public class Template2006 {
 
    public static void main(String[] args) throws IOException {
    	Event actEvent;
    	State actState = new State();
    	new EventList();
        EventList.InsertEvent(G.ARRIVAL, 1);
        EventList.InsertEvent(G.MEASURE, 5);
    	while (G.time < 100000){
    		actEvent = EventList.FetchEvent();
    		G.time = actEvent.eventTime;
    		actState.TreatEvent(actEvent);
    	}
    	System.out.println(1.0*actState.accumulated/actState.noMeasurements);
    	System.out.println(actState.accumulated);
    	System.out.println(actState.noMeasurements);
    	actState.W.close();
    }
}
import java.io.*;

/*
 * SimpleFileWriter.java
 *
 * Mikael Andersson, Lund Institute of Technology, 2002-2005
 *
 */
 
public class SimpleFileWriter 
{
	FileWriter aFileWriter;
	
	String aNewLine = "\r\n";
	
	/*
	 * Creates a new SimpleFileWriter
	 *
	 * @param	pFileName		The filename to use
	 * @param	pAppend			Whether to append to existing data in file
	 *
	 */
	public SimpleFileWriter(String pFileName,
							boolean pAppend)
	{
		try
		{
			aFileWriter = new FileWriter(pFileName, pAppend);
		}
        catch(IOException pIOE)
        {
        	System.out.println(pIOE.toString());
        }
	}
	
	/*
	 * Closes the fileprinter
	 *
	 */
	public void close()
	{
		try
		{
			aFileWriter.close();
		}
		catch(IOException pIOE)
		{
			System.out.println(pIOE.toString());	
		}
	}
	
	/*
	 * Appends a new line to the file
	 *
	 * @param	pString		The string to append to the file
	 *
	 */
	public void println(String pString)
	{		
		try
		{
			aFileWriter.write(pString, 0, pString.length());
			aFileWriter.write(aNewLine, 0, aNewLine.length());
        }
        catch(IOException pIOE)
        {
        	System.out.println(pIOE.toString());
        }       
	}	
	
	/*
	 * Appends a text to the file
	 *
	 * @param	pString		The string to append to the file
	 *
	 */
	public void print(String pString)
	{		
		try
		{
			aFileWriter.write(pString, 0, pString.length());
        }
        catch(IOException pIOE)
        {
        	System.out.println(pIOE.toString());
        }       
	}	
}
