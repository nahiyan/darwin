<template>
  <div>
    <Navigation :extension="extension"></Navigation>
    <h1>Sessions of {{ extension.name }}</h1>
    <table class="table mt-4 table-striped" v-if="sessions.length > 0">
      <thead>
        <tr>
          <th scope="col">#</th>
          <th scope="col">ID</th>
          <th scope="col">Actions</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="session in sessions" v-bind:key="session.id">
          <th scope="row">{{ session.index }}</th>
          <td scope="row">{{ session.id }}</td>
          <td>
            <router-link
              :to="{ name: 'generations', params: { id: session.id } }"
              >Generations</router-link
            >
            <!-- &nbsp; -->
            <!-- <a v-on:click.prevent="getGenerations(session.id)" href="#"
              >Scores</a
            > -->
          </td>
        </tr>
      </tbody>
    </table>
    <ItemNotFound v-if="sessions.length == 0" />
  </div>
</template>

<script>
import ordinal from "ordinal";
import ItemNotFound from "./ItemNotFound";
import Navigation from "../vue/Navigation";

export default {
  data() {
    return {
      sessions: [],
      extension: {},
    };
  },
  mounted() {
    window.axios
      .get("/api/sessions/" + this.$route.params.id)
      .then((response) => {
        const data = response.data;
        this.sessions = data.sessions;
        this.extension = data.extension;
        let i = this.sessions.length;
        this.sessions.forEach((session) => {
          session.index = ordinal(i);
          i -= 1;
        });
      });
  },
  components: {
    ItemNotFound,
    Navigation,
  },
  props: ["selection"],
};
</script>