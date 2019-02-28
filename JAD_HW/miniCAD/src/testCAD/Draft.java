package testCAD;

import java.awt.Graphics;
import java.util.ArrayList;

import javax.swing.JFrame;
import javax.swing.JPanel;

import testCAD.Listener;

class Draft extends JPanel{
	private static Listener listener = new Listener();
	public Draft() {
		super();
        this.addMouseListener(listener);
        this.addMouseMotionListener(listener);
        this.addMouseWheelListener(listener);
	} 
	public JFrame drawGraphic(int i) {
		JFrame newframe = new JFrame();
		Listener L = new Listener();
		return newframe;
	}
	
	 public void printAllShape(ArrayList<Shapes> container) {  //ÖØÐÂ»æÖÆÍ¼²ã
	    Graphics graphics = this.getGraphics();
	    graphics.clearRect(0,0,840,600);
	    for(Shapes shape: container){
	    	shape.draw(graphics);
	    }
	}

}
