package group12.sparc_moblieapp;

import android.os.Parcel;
import android.os.Parcelable;


//@SuppressWarnings("serial") //With this annotation we are going to hide compiler warnings


public class Schedule implements Parcelable {
    public Schedule(String Day, String Time){
        this.Day = Day;
        this.Time = Time;
    }
    public String getDay() {
        return this.Day;
    }
    public String getSTime(){
        return this.Time;
    }

    public void setDay(String Day){
        this.Day = Day;
    }

    public void setSTime(String Time){
        this.Time = Time;
    }



    private String Day;
    private String Time;

    public Schedule(Parcel in){
        String[] data = new String[2];
        in.readStringArray(data);
        this.Day = data[0];
        this.Time = data[1];
    }

    @Override
    public int describeContents(){
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags){
        dest.writeStringArray(new String[]{this.Day,this.Time});
    }
    public static final Parcelable.Creator CREATOR = new Parcelable.Creator(){
        public Schedule createFromParcel(Parcel in){
            return new Schedule(in);
        }

        public Schedule[] newArray(int size){
            return new Schedule[size];
        }
    };

}
