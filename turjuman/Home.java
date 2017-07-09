package turjuman;

import java.net.URL;
import java.util.ResourceBundle;
import java.util.ArrayList;
import javafx.fxml.Initializable;
import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleGroup;
import javafx.scene.control.RadioButton;
import javafx.scene.text.Text;
import javafx.scene.text.Font;
import javafx.scene.layout.VBox;
import javafx.event.ActionEvent;


public class Home implements Initializable{
		

	@FXML private TextField   wordInput; 
	@FXML private RadioButton russianButton;
	@FXML private RadioButton englishButton;
	@FXML private VBox        wordsList;


	private ToggleGroup languageButtons = new ToggleGroup();
	private ArrayList<String> russianWords = new ArrayList<>();
	private String            englishWord  = new String(); 
	private DatabaseConnection sqlite;

	@FXML
	public void initialize(URL url, ResourceBundle bundle){
		try{
			this.sqlite = new DatabaseConnection();
		}catch(Exception e){
			System.out.println(e.getMessage());
		}
		englishButton.setToggleGroup(languageButtons);
		russianButton.setToggleGroup(languageButtons);
	}

	@FXML
	private void translateWordAction(ActionEvent event){
		try{
			translateWord();			
		}catch(Exception e){
			System.out.println(e.getMessage());
		}
	}

	public void translateWord()throws Exception{
		// Clears the wordsList (VBox) before adding any new words.
		clearWordsList();

		RadioButton languageButton = (RadioButton) languageButtons.getSelectedToggle();

		if(languageButton == null){
			// No language was selected. Returning from function.
			return;
		}
		String selectedLanguage = languageButton.getId();
		String word           = wordInput.getText();


		// Deciding which is the original language.
		if(selectedLanguage.equals("russianButton")){
			englishWord   = sqlite.translateRussianWord(word);				
	
			Text wordItem = new Text(englishWord);
			wordItem.setFont(Font.font("Verdana", 20));

			wordsList.getChildren().add(wordItem);

		}else if(selectedLanguage.equals("englishButton")){

			// Returns an arraylist of russian words.
			russianWords = sqlite.translateEnglishWord(word);
			// Distributing the content of russianWords (ArrayList) over wordsList(VBox)
			for(int i = 0; i < russianWords.size(); i ++){
				Text wordItem = new Text(russianWords.get(i));
				wordItem.setFont(Font.font(null, 20));
				wordsList.getChildren().add(wordItem);
			}
		}

	}

	public void clearWordsList(){
		wordsList.getChildren().clear();
	}

}