package com.example.tcpAesCrc;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.Socket;
import java.util.Arrays;

import com.example.aes128.AES128;
import com.example.crc32.crc32;

/*--------TCP/CLIENT/AES128/CRC32---------
 * 
 * Name: 			HECTOR CARLOS DE JESUS FLORES FLORES
 * e-mail:			carlos.flogars@gmail.com
 * Date:			12/February/2019
 * 
 * Description: This module test a tcp client, using the 
 * standard encryption of AES128 and the checksum CRC32.
 * Basically there is two functions at the bottom that received
 * and sent messages with AES128 and CRC32 you only need to pass
 * them the message and the key and they will do all the work.
 * 
 *---------------------------------------- */


public class TestClient {

	public static void main(String[] args) throws Exception {

		/*Create a socket*/
		Socket sock = new Socket("192.168.0.101",50000);
		
		/*Print when client has connected*/
		System.out.println("Connected to the server");
		
		/*Create the input stream*/
		InputStream in = sock.getInputStream();
		
		/*Create the output stream*/
		OutputStream out = sock.getOutputStream();
		
		/*Print to now that the message is sending*/
		System.out.println("Sending product info...");
		
		/*Write in the socket the message encrypted and with crc*/
		out.write(aesCrcSend("Hola que tal mun","1111111111111111"));
		
		/*Create byte array to catch the message*/
		byte response[] = new byte[26];
		
		/*Listen the input stream*/
		in.read(response);
		
		/*Verify the received message and printing it out*/
		aesCrcReceive(response,"1111111111111111");
		
		/*Close the socket*/
		sock.close();
		
	}
	
	private static byte[] aesCrcSend(String stringAES, String key) throws UnsupportedEncodingException, Exception {
		
		/*Encrypting the message of 16 bytes*/
		byte a[] = AES128.ecbEncrypt(stringAES.getBytes("UTF-8"),key);
		
		/*Getting the crc of the encrypted message*/
		byte b[] = Long.toString(crc32.getCrc32(AES128.ecbEncrypt(stringAES.getBytes("UTF-8"),key))).getBytes();
		
		/*Creating the new byte array to save the encrypted message with the crc*/
		byte[] c = new byte[a.length + b.length];
		
		/*copy byte array a to c*/
		System.arraycopy(a, 0, c, 0, a.length);
		
		/*copy byte array b to c*/
		System.arraycopy(b, 0, c, a.length, b.length);
		
		return c;
	}
	
	private static void aesCrcReceive(byte[] data, String key) throws UnsupportedEncodingException, Exception {

		/* Create byte array to store the message encrypted received from the client */
		byte[] messageByte = Arrays.copyOfRange(data, 0, 16);

		/* Create byte array to store the crcChecksum received from the client */
		byte[] crcByte = Arrays.copyOfRange(data, 16, 25);

		/* Convert from byte array to string */
		String crcString = new String(crcByte, "UTF-8");

		/* Check the message calculating the crc32 */
		String messageString = Long.toString(crc32.getCrc32(messageByte));

		/*Cut the message to take just 9 bytes*/
		messageString = messageString.substring(0, 9);

		/* Compare two crc, the calculated and the received from the client */
		if (0 == messageString.compareTo(crcString)) {
			System.out.println("The message received arrived COMPLETE!!!");
		} else {
			System.out.println("The message received ARRIVED INCOMPLETE!!!");
		}

		/* Decripting the message and pass it to an arrray */
		byte[] decryptedCipherText = AES128.ecbDecrypt(messageByte, key);

		/* Conver the message from bytes to String */
		String strResponse = new String(decryptedCipherText).trim();

		/* Conver the message from bytes to String */
		String information = new String(messageByte).trim();

		/* Print message received */
		System.out.println("The message received with encryption is: " + information);

		/* Print message received */
		System.out.println("The message received without encryption is: " + strResponse);


	}

}
