package turjuman;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.Parent;
import javafx.scene.Node;
import javafx.stage.Stage;
import javafx.fxml.FXMLLoader;
import java.io.IOException;

public class Main extends Application{
	public static void main(String[] args) {
		launch(args);
	}

	@Override
	public void start(Stage stage){
		try{
			Parent parent = FXMLLoader.load(getClass().getResource("Home.fxml"));
			Scene  scene  = new Scene(parent);


			stage.setScene(scene);
			stage.setTitle("Turjuman");
			stage.show();		
		}catch(IOException e){
			System.out.println(e);
		}
	}
}