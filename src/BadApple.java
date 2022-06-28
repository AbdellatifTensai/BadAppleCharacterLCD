import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JButton;
import javax.swing.JFrame;

import jssc.SerialPort;
import jssc.SerialPortException;

class BadApple{
    
    static BufferedImage bufferedImage;
    String output;
    int index;
    static SerialPort serialPort;
    int ndx;
    String input;
    Thread t = new Thread(){
    	@Override public void run() {
    		while(serialPort.isOpened()) {
    			try {
					input = serialPort.readString();
					System.out.println(input ==null? "":input);
					Thread.sleep(50);
				}catch (InterruptedException e){e.printStackTrace();}
		    	catch (SerialPortException e){e.printStackTrace();}
    		}
    	}
    };
    
    public static void main(String[] args){
		BadApple badApple = new BadApple();
		badApple.initialize();
		badApple.setUpGUI();
    }
    
    private void readData() {
        output = "<";
    	for(int z=0;z<4;z++){
	    	for(int y=0;y<bufferedImage.getHeight();y++){
		    	ndx = z*5;
	    		while(ndx< (5*z)+5){
	    			output += bufferedImage.getRGB(ndx, y) == -1? "0" : "1";
	    			output += (ndx+1) % 5 == 0 ? ">" + index : "";
	    			index = (ndx+1) % 5 ==0? index +1 : index +0;
	    			output += (ndx+1) % 5 == 0 ? "<":"";
	    			ndx++; 
    			}
//	    			System.out.println(output);
	    			try {
	    			serialPort.writeString(output);
	    			}catch(SerialPortException e){e.printStackTrace();} 
//		    		catch (InterruptedException e){e.printStackTrace();}
			    	output = "";
	    	}
    	}
    
    }
  
    
    private void initialize(){
    	serialPort = new SerialPort("COM9");
    	try{
    		serialPort.openPort();
    		serialPort.setParams(SerialPort. BAUDRATE_115200, 
    				SerialPort.DATABITS_8, 
    				SerialPort.STOPBITS_1, 
    				SerialPort.PARITY_NONE);
       
    	}catch(Exception e){e.printStackTrace();}
    }
    
    private void setUpGUI(){
    	JFrame frame = new JFrame("Bad Apple!");
    	JButton sendButton = new JButton("Send");
    	sendButton.addActionListener(new ActionListener(){
    		@Override public void actionPerformed(ActionEvent ev) {
    			if(serialPort.isOpened()){
    				for(int x=1;x<=4600;x++){
    		    		try {
    		    			bufferedImage = ImageIO.read(new File("D:/Dev/BadAppleCharacterLCD/resources/frames21_40%/frame" + x +".png"));
    		    		}catch(IOException e){e.printStackTrace();}
    					readData();
    					index = 0;
    				}
    			}
    		}
    	});
    	frame.add(sendButton);
    	frame.setVisible(true);
    	frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    	frame.setSize(300,300);
    }
}

/*00000 00000 00000 01000
 *00000 00000 00000 00000
 *00000 00000 00000 00000
 *00000 00000 00000 01111
 *00000 00000 00000 11110
 *00000 00000 00000 00000
 *00000 00000 00000 00111
 *00000 00000 00000 11111
 *
  00111 11111 01111 11101
  10011 11111 11111 00000
  11111 11111 11111 00000
  10111 11111 10111 11110
  11111 11111 11111 11110
  00111 11111 11111 00010
  11011 11111 11001 00000
  11111 11111 11111 00000*/

/*00000 00000 00011 11111
 *00000 00000 00000 11111
 *00000 00000 00011 00011
 *00000 00000 01111 00000
 *00000 00000 11111 11000
 *00000 00000 11111 11100
 *00000 00000 11111 11100
 *00000 00000 11111 11100
 *
 *00000 00000 01111 11100
 *00000 00011 01111 11100
 *00000 00111 11111 11110
 *00000 00011 10111 11110
 *00000 00011 10111 11110
 *00000 00011 11111 11110
 *00000 00011 11111 11000
 *00000 00011 11111 11100
 *
 *110001100011010
 *
 *
*/ 

/*00000 00000 00011 00000
  00000 00000 01111 11000
  00000 00000 11111 11100
  00000 00000 11111 11100
  00000 00000 11111 11100
  00000 00000 11111 11100
  00000 00000 01111 11100
  00000 00000 01111 11110
  
  00000 00011 11111 11110
  00000 00111 10111 11110
  00000 00011 10111 11110
  00000 00011 11111 11000
  00000 00011 11111 11100
  00000 00011 11111 11000
  00000 00011 11111 11000
  00000 00011 11111 11010*/

//00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011001110001100011000110001100011000110001101111111111111111111111110111101111111111011110111111111111111111111111111100000110001110011100111001110011100111101111011110111101100011100110001100011010
