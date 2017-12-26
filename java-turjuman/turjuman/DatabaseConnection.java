package turjuman;
/*
This class is responsible for connecting to the database, 
	and translating both english and russian words.
*/

import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.Statement;
import java.sql.SQLException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.util.ArrayList;

public class DatabaseConnection{

	private Connection connection;

	DatabaseConnection(){
		try{
			this.connect("dictionary.db");			
		}catch(SQLException e){
			System.out.println(e.getMessage());
		}
	}

	DatabaseConnection(String url){
		try{
			this.connect(url);
		}catch(SQLException e){
			System.out.println(e.getMessage());
		}
	}

	public void connect(String url)throws SQLException{
		this.connection = DriverManager.getConnection("jdbc:sqlite:" + url);
	}

	public ArrayList<String> translateEnglishWord(String englishWord)throws SQLException{
		String            sql   = "select russian from words where english = ?";
		PreparedStatement query = this.connection.prepareStatement(sql);
		query.setString(1, englishWord);

		ResultSet result = query.executeQuery();
			
		ArrayList<String> words = new ArrayList<>();	
		while(result.next()){
			words.add(result.getString("russian"));
		}
		result.close();
		query.close();
		// System.out.println(words.get(0));
		return words;
	}

	public String translateRussianWord(String russianWord)throws SQLException{
		String sql = "select english from words where russian = ?";
		PreparedStatement query = this.connection.prepareStatement(sql);
		query.setString(1, russianWord);

		ResultSet result = query.executeQuery();
		result.next();
		String word = result.getString("english");
		// System.out.println(word);
		return word;
	}

	public void close(){
		try{
			this.connection.close();
		}catch(SQLException e){
			System.out.println(e.getMessage());
		}
	}
} 