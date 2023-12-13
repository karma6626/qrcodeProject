#pragma once
#include <cliext/list>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Data::SqlClient;
using namespace System::Collections::Generic; // Include this for List
using namespace cliext;



ref class Connection {
private:
	static String^ connstring = "Data Source=localhost\\SQLEXPRESS;Initial Catalog=database1;Integrated Security=True;";
public:
	static void Create(String^ name, String^ email, String^ phone, String^ address, String^ password, String^ contactperson, String^ contactno) {

		try {

			SqlConnection sqlConn(connstring);
			sqlConn.Open();

			String^ sqlQuery = "INSERT INTO users " +
				"(name, email, phone, address, password,contactperson,contactno) VALUES" +
				"(@name, @email, @phone, @address, @password, @conPerson, @conNo);";

			SqlCommand command(sqlQuery, % sqlConn);
			command.Parameters->AddWithValue("@name", name);
			command.Parameters->AddWithValue("@email", email);
			command.Parameters->AddWithValue("@phone", phone);
			command.Parameters->AddWithValue("@address", address);
			command.Parameters->AddWithValue("@password", password);
			command.Parameters->AddWithValue("@conPerson", contactperson);
			command.Parameters->AddWithValue("@conNo", contactno);

			command.ExecuteNonQuery();

			MessageBox::Show("Successfully registered",
				"Success", MessageBoxButtons::OK);
		}
		catch (Exception^ ex) {
			MessageBox::Show("Failed to register new user",
				"Register Failure", MessageBoxButtons::OK);
		}

	}
	static bool IsEmailTaken(String^ email) {
		try {
			SqlConnectionStringBuilder^ builder = gcnew SqlConnectionStringBuilder();
			builder->ConnectionString = "Data Source=localhost\\SQLEXPRESS;Initial Catalog=database1;Integrated Security=True;";

			SqlConnection^ sqlConnection = gcnew SqlConnection(builder->ConnectionString);
			sqlConnection->Open();

			SqlCommand^ cmd = gcnew SqlCommand("SELECT COUNT(*) FROM users WHERE email = @email", sqlConnection);
			cmd->Parameters->AddWithValue("@email", email);

			int count = safe_cast<int>(cmd->ExecuteScalar());

			sqlConnection->Close();

			// If count is greater than 0, the email is already taken
			return count > 0;
		}
		catch (Exception^ ex) {
			MessageBox::Show(ex->Message, "Database Error", MessageBoxButtons::OK);
			return false;
		}

}
	static bool IsPhoneTaken(String^ phone) {
		try {
			SqlConnectionStringBuilder^ builder = gcnew SqlConnectionStringBuilder();
			builder->ConnectionString = "Data Source=localhost\\SQLEXPRESS;Initial Catalog=database1;Integrated Security=True;";

			SqlConnection^ sqlConnection = gcnew SqlConnection(builder->ConnectionString);
			sqlConnection->Open();

			SqlCommand^ cmd = gcnew SqlCommand("SELECT COUNT(*) FROM users WHERE Phone = @Phone", sqlConnection);
			cmd->Parameters->AddWithValue("@phone", phone);

			int count = safe_cast<int>(cmd->ExecuteScalar());

			sqlConnection->Close();

			// If count is greater than 0, the phone number is already taken
			return count > 0;
		}
		catch (Exception^ ex) {
			MessageBox::Show(ex->Message, "Database Error", MessageBoxButtons::OK);
			return false;
		}
	}
	


	static void Reload_DataGrid(Object^% dataTable, String^ query) {
		SqlConnection^ sqlConn = gcnew SqlConnection(connstring);
		sqlConn->Open();

		String^ sqlQuery = "SELECT Id,name,email,phone,address,contactperson,contactno FROM " + query;
		SqlDataAdapter^ da = gcnew SqlDataAdapter(sqlQuery, sqlConn);
		DataTable^ dt = gcnew DataTable();
		da->Fill(dt);

		dataTable = dt;
	}
	// Assuming Reload_DataGrid is a static function in the Connection class
	static void Connection::Reload_DataGrid1(DataTable^% dataTable, String^ query) {
		SqlConnection^ sqlConn = gcnew SqlConnection(connstring);
		sqlConn->Open();

		String^ sqlQuery = "SELECT Id, name, email, phone, address, contactperson, contactno FROM " + query;
		SqlDataAdapter^ da = gcnew SqlDataAdapter(sqlQuery, sqlConn);
		DataTable^ dt = gcnew DataTable();
		da->Fill(dt);

		dataTable = dt;
	}


	static void Search_DataGrid(Object^% dataTable, String^ searchText) {
		SqlConnection^ sqlConn = gcnew SqlConnection(connstring);
		sqlConn->Open();

		String^ sqlQuery = "SELECT Id, name, email, phone, address, contactperson, contactno FROM users WHERE Id=@id OR name LIKE @searchText";

		SqlCommand^ command = gcnew SqlCommand(sqlQuery, sqlConn);

		try {
			int id;
			if (Int32::TryParse(searchText, id)) {
				command->Parameters->AddWithValue("@id", id);
			}
			else {
				// Handle the case where searchText is not a valid integer
				command->Parameters->AddWithValue("@id", DBNull::Value);
			}
		}
		catch (...) {
			// Handle other exceptions if needed
			command->Parameters->AddWithValue("@id", DBNull::Value);
		}

		command->Parameters->AddWithValue("@searchText", "%" + searchText + "%"); // Use LIKE for partial name matches

		SqlDataAdapter^ da = gcnew SqlDataAdapter(command);
		DataTable^ dt = gcnew DataTable();
		da->Fill(dt);
		dataTable = dt;
	}




	static void Update(int id, String^ name,String^ email, String^ phone, String^ address, String^ conPerson, String^ conNo) {
		try {
			SqlConnection sqlConn(connstring);
			sqlConn.Open();

			String^ sqlQuery = "UPDATE users SET name=@name,email=@email,phone=@phone, address=@address, contactperson=@conPerson, contactno=@conNo WHERE Id=@id";

			SqlCommand command(sqlQuery, % sqlConn);
			command.Parameters->AddWithValue("@id", id);
			command.Parameters->AddWithValue("@name", name);
			command.Parameters->AddWithValue("@email", email);
			command.Parameters->AddWithValue("@phone", phone);
			command.Parameters->AddWithValue("@address", address);
			command.Parameters->AddWithValue("@conPerson", conPerson);
			command.Parameters->AddWithValue("@conNo", conNo);

			command.ExecuteNonQuery();

			MessageBox::Show("Successfully Updated",
				"Success", MessageBoxButtons::OK);

		}
		catch (Exception^ ex) {
			MessageBox::Show("Failed to register new user",
				"Register Failure", MessageBoxButtons::OK);
		}
	}

	static void Delete(String^ id) {
		try {
			int Id = System::Int32::Parse(id);

			SqlConnection sqlConn(connstring);
			sqlConn.Open();

			String^ sqlQuery = "DELETE users WHERE Id=@id";

			SqlCommand command(sqlQuery, % sqlConn);
			command.Parameters->AddWithValue("@id", Id);

			command.ExecuteNonQuery();

			MessageBox::Show("Successfully Deleted",
				"Success", MessageBoxButtons::OK);

		}
		catch (Exception^ ex) {
			MessageBox::Show("Failed to register new user",
				"Register Failure", MessageBoxButtons::OK);
		}
	}



};
