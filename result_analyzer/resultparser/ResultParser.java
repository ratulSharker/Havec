/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package resultparser;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.StringTokenizer;

/**
 *
 * @author Ra2l
 */
public class ResultParser {

    /*
    some constant to manage things
    */
    //final public String folderName = "all_results";
    
    private File[] unprocessedFileList;
    private String inputFolder;

    public ResultParser(String folder) {
        
        //saving the folder name
        inputFolder = folder;

        //populating the file list
        boolean populateSuccess = this.populateUnprocessedList();
        
        if(populateSuccess)
        {
	        int i=0;
	        for (File f : this.unprocessedFileList) {
	            //System.out.println(f.getName() + "   " + this.getMultiplyingFactorFromFilename(f.getName()));
	            this.processSingleFile(f,i++);
	        }
        }


    }
    
    private boolean populateUnprocessedList(){
        File dir = new File(inputFolder);

        if(dir.exists())
        {
	        this.unprocessedFileList = dir.listFiles();
	        return true;
        }
        else
        {
        	System.err.println("**** NO FOLDER NAMED " + inputFolder + " FOUND ****");
        	return false;
        }

    }
    
    
    
    private void processSingleFile(File f,int index){
        
        //multiply by management -- depends on file name
        /***********************************************/
        String multipliedBy = this.getMultiplyingFactorFromFilename(f.getName());
        /***********************************************/
        
        //date management
        /**********************************************/
        Date startTime = new Date(),
                endTime = new Date();
        boolean firstDateConsumed = false;
        boolean lastDateConsumed = false;
        DateFormat df = new SimpleDateFormat("yyyy-MM-dd.HH:mm:ss");
        /**********************************************/
        
        //params ->filename kSize hSize
        /**********************************************/
        String fileName="";
        int kSize=0;
        long hSize=0;
        boolean paramsConsumed = false;
        int hashFunc = 8;

/*
        int hashInfoIndex = f.getName().length() - (5);
//            System.out.println("hello " + f.getName().substring(hashInfoIndex, hashInfoIndex+1));
        hashFunc = Integer.parseInt(f.getName().substring(hashInfoIndex, hashInfoIndex+1));
*/
        /**********************************************/
        
        //output result
        /**********************************************/
        double lastMemSize = 0;//in Gb
        double lastHashMemSize = 0;//in Gb
        double lastVectorMemSize = 0;//in Gb
        long disticntInMem  = 0;
        long disticntInVct  = 0;
        long totalInMem = 0;
        long totalInVect = 0;
        long khmerFound     = 0;
        long khmerNotFound  = 0;
        /**********************************************/
        
        try{
            
            FileInputStream fr = new FileInputStream(f);//opening the given file
            BufferedReader br = new BufferedReader(new InputStreamReader(fr));
            String line;
            
            while((line = br.readLine()) != null){
                
                
                /********PARSING THE DATE************/
                if(line.startsWith("current time ::")){
                    //System.out.println(line.substring(15));
                    Date tempDate = df.parse(line.substring(15));
                    if(firstDateConsumed == false){
                        //it is the first date
                        startTime = tempDate;
                        firstDateConsumed = true;
                        //System.out.println("###START_TIME###"+line+"###");
                    }
                    else if(lastDateConsumed == false){
                        endTime = tempDate;
                        //System.out.println("###END_TIME###"+line+"### ==> " + (endTime.getTime() - startTime.getTime())/1000);
                    }
                }
                else if(line.startsWith("stat from consume function ::")){
                    lastDateConsumed = true;
                }
                /***********************************/
                
                /******PARAM PARSING*********/
                if(line.startsWith("filename::") && paramsConsumed == false){
                    
                    paramsConsumed = true;//ensuring that only first time it enters
                    
                    StringTokenizer tokenizer = new StringTokenizer(line, " ");
                    fileName = tokenizer.nextToken().substring("filename::".length());//"filename::../dataset1_7.GB"
                    
                    String temp = tokenizer.nextToken().substring("ksize::".length());
                    kSize = Integer.parseInt(temp, 10);
                    
                    temp = tokenizer.nextToken().substring("Size::".length());
                    hSize = Long.parseLong(temp);
                    
                    /////////////////////////////////////////////////////////////                    
                    //int structureSize = this.is5ByteStructureFromFilename(f.getName()) ? 5 : 6;
                    //lastHashMemSize = (double)(hSize * structureSize)/(double)(1024.0f*1024.0f*1024.0f);//getting the actual Gb size (delete)
                    /////////////////////////////////////////////////////////////
                }
                /***********************************/
                
                /******RESULT PARSING*********/
                if(line.startsWith("Total memory used (GB):  ")){
                    //time to consume -- memSize
                    String memString = line.substring("Total memory used (GB):  ".length());
                    lastMemSize = Double.parseDouble(memString);
                    
                    //lastVectorMemSize = lastMemSize - lastHashMemSize;
                }

                if(line.startsWith("HashTable memory allocation (GB):: "))
                {
                    String memString = line.substring("HashTable memory allocation (GB):: ".length());
                    lastHashMemSize = Double.parseDouble(memString);
                }
                if(line.startsWith("Vector memory allocation    (GB):: "))
                {
                    String memString = line.substring("Vector memory allocation    (GB):: ".length());
                    lastVectorMemSize = Double.parseDouble(memString);
                }
                
                if(line.startsWith("khmer in memBlock (found):: ")){
                    String temp = line.substring(28);
                    totalInMem = Long.parseLong(temp);
                }
                if(line.startsWith("khmer in list (found)::")){
                    String temp = line.substring(24);
                    totalInVect = Long.parseLong(temp);
                }
                if(line.startsWith("	khmer found :: ")){
                    String temp = line.substring(16);
                    khmerFound = Long.parseLong(temp);
                }
                if(line.startsWith("	khmer not found:: ")){
                    String temp = line.substring(19);
                    khmerNotFound = Long.parseLong(temp);
                }
                if(line.startsWith("khmer in memBlock (distinct)::")){
                    String temp = line.substring(31);
                    disticntInMem = Long.parseLong(temp);
                }
                if(line.startsWith("khmer in list (distinct)::")){
                    String temp = line.substring(27);
                    disticntInVct = Long.parseLong(temp);
                }
                /***********************************/
            }
            
            
        }
        catch(Exception ex){
            System.out.println("ERROR IN PROCESSING A FILE " + ex.getMessage());
        }
        finally{          
            
            /*
            if(startTime.getHours() == 11
                    && endTime.getHours() != 11){
                startTime.setHours(0);
                endTime.setHours(endTime.getHours()+1);
                //System.out.println(startTime + " " + endTime );
            }
            */
            
            //time thing
            long elapsedMili = endTime.getTime() - startTime.getTime();
            //long min = (elapsedMili / (1000*60));
            //elapsedMili = elapsedMili % (1000*60);//mod by minute -- whatever remainder is for the second
            long sec = (elapsedMili / (1000));
            //System.out.print("<td>" + min + " m " + sec + " s</td>");
            
            
             DecimalFormat decFor = new DecimalFormat("0.0000");
            //result thing
            
            //(enable)
            System.out.println("<tr>"
                    + "<td>" + index + "</td>"
                    + "<td>" + fileName + "</td>"
                    + "<td>" + (this.is5ByteStructureFromFilename(f.getName()) ? "5 byte" : "6 byte") + "</td>"
                    + "<td>" + kSize + "</td>"
                    + "<td>" + hSize + "</td>"
                    + "<td>" + hashFunc + "</td>"
                    + "<td>" + sec + "</td>"
                    //+"<td>" + disticntInMem +"</td>"
                    //+ "<td>"+ disticntInVct + "</td>"
                    //+ "<td>" + ((khmerFound == 0) ? "NA" : khmerFound) + "</td>"    // giving not available
                    + "<td>"+ lastMemSize  + "</td>"
                    
                    
                    + "<td>" + decFor.format(lastHashMemSize / lastMemSize * 100.0f) + " %" + "</td>"
                    + "<td>" + decFor.format(lastVectorMemSize / lastMemSize * 100.0f) + " %" +"</td>"
                    
                    //+ "<td>"+ khmerNotFound + "</td>"
                    + "<td>" + multipliedBy + "</td>"
                    + "<td>" + disticntInMem + "</td>"
                    + "<td>" + disticntInVct +"</td>"
                    + "<td>" + (disticntInMem + disticntInVct) +"</td>"
                    + "<td>"+decFor.format((double)disticntInMem / (double)hSize)+ "</td>"
                    + "<td>"+decFor.format((double)(disticntInMem + disticntInVct) / (double)hSize) + "</td>"
                    + "<td>"+decFor.format((double)hSize / (double)(disticntInMem + disticntInVct))+"</td>"
                    + "</tr>");
            
            /*
            System.out.println("<td>" + disticntInMem +"</td>"
                    + "<td>"+ disticntInVct + "</td>"
                    + "<td>" + khmerFound + "</td>"
                    + "<td>"+ ((lastMemSize*1024)- hSize)  + "</td>"
                    + "<td>"+ khmerNotFound + "</td>"
                    + "<td>" + multipliedBy + "</td>"
                    + "<td>" + disticntInMem + "</td>"
                    + "<td>" + disticntInVct +"</td>"
                    + "<td>" + (disticntInMem + disticntInVct) +"</td>"
                    + "<td>"+decFor.format((double)disticntInMem / (double)hSize)+ "</td>"
                    + "<td>"+decFor.format((double)(disticntInMem + disticntInVct) / (double)hSize) + "</td>"
                    + "<td>"+decFor.format((double)hSize / (double)(disticntInMem + disticntInVct))+"</td>"
                    + "</tr>");
            */
        }
    }
    
    
    
    
    //
    // Helper small functionality
    //

    
    // This function parse the filename for the multiplying factor
    // if the file naming convention is changed, please 
    // you must update this function in accordance to get the correct
    // result sheet
    private String getMultiplyingFactorFromFilename(String filename)
    {
        String multipliedBy="1";
        if(filename.contains("_mf_")){
            //if they present -> that means that it got some multiplication
            
            int startingIndex = filename.indexOf("_mf_") + "_mf_".length();
            int endingIndex = filename.length() - ".txt".length();
            
            multipliedBy = filename.substring(startingIndex, endingIndex);
            
            multipliedBy = multipliedBy.replace('-', '.');
        }
        
        return multipliedBy;
    }
    
    
    // This function parse the filename for the 5 byte / 6 byte
    // data structure used for storing each khmer
    // you must update this function in accordance to get the 
    // correct result sheet
    private boolean is5ByteStructureFromFilename(String filename)
    {
        return filename.startsWith("5_");
    }
    
    
    
    
    
    
    public static void main(String[] args) {
        
        if(args.length == 1)
        {
	        System.out.println("<html>"
	                + "<head>"
	                + "<style>" 
	                +" table td{ " 
	                +" padding: 10px; text-align: center; } "
	                +" </style> "
	                +"</head> <body> <table border='2'>");
	        
	        System.out.println("<tr>"
	                + "<td>~#~</td>"
	                + "<td><pre>Filename</pre></td>"
	                + "<td><pre>Structure Type</pre></td>"
	                + "<td><pre>kSize</pre></td>"
	                + "<td><pre>index in hash table</pre></td>"
	                + "<td><pre># of hash function</pre></td>"
	                + "<td><pre>run time (sec)</pre></td>"
	                //+ "<td>in table</td>"
	                //+ "<td>in vector</td>"
	                //+ "<td>Total khmer found</td>"
	                + "<td><pre>Total mem (GB)</pre></td>"
	                + "<td><pre>Hash mem (% of total)<pre></td>"
	                + "<td><pre>Vct mem (% of total)<pre></td>"
	                //+ "<td>not found</td>"
	                + "<td><pre>multiply-by</pre></td>"
	                + "<td><pre>solid in mem </pre></td>"
	                + "<td><pre>solid in vct</pre></td>"
	                + "<td><pre>solid total</pre></td>"
	                + "<td><pre>khmer in mem / # of hash index</pre></td>"
	                + "<td><pre>total khmer / # of hash index</pre></td>"
	                + "<td> <pre># of hash index / total khmer</pre></td>"
	                + "</tr>" );
	        ResultParser rp = new ResultParser(args[0]);
	        System.out.println("</table> </body> </html>");
        }
        else if(args.length == 0)
        {
        	System.err.println("**** NO INPUT DIRECTORY SPECIFIED ****");
        }
        else //args.size() > 1
        {
        	System.err.println("**** MORE THAN ONE INPUT DIRECTORY SPECIFIED ****");
        }
    }
}
