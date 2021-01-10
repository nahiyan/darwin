<template>
  <div>
    <Navigation :session="session" :extension="session.extension"></Navigation>
    <h1>Generations of Session {{ session.id }}</h1>

    <div class="card">
      <div class="card-body">
        <h5 class="card-title">Import Generation</h5>
        <form action="#" @submit.prevent="import_">
          <input type="file" @change="selectFile" />
          <input
            type="submit"
            value="Import"
            class="btn btn-outline-success btn-sm"
          />
        </form>
      </div>
    </div>

    <table
      class="table mt-4 table-striped"
      v-if="generations.length > 0 && loaded"
    >
      <thead>
        <tr>
          <th scope="col">#</th>
          <th scope="col">ID</th>
          <th scope="col">Actions</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="generation in generations" v-bind:key="generation.id">
          <th scope="row">{{ generation.index }}</th>
          <td scope="row">{{ generation.id }}</td>
          <td>
            <router-link
              :to="{
                name: 'generation_scores',
                params: { id: generation.id },
              }"
              >Scores</router-link
            >&nbsp;
            <a :href="'/api/generation/' + generation.id + '/export'">Export</a>
          </td>
        </tr>
      </tbody>
    </table>
    <ItemNotFound v-if="generations.length == 0 && loaded" />
  </div>
</template>

<script>
import ordinal from "ordinal";
import ItemNotFound from "./ItemNotFound";
import Navigation from "../vue/Navigation";

export default {
  methods: {
    import_() {
      const data = new FormData();
      data.append("generation_state", this.generationState);
      axios.post(`/api/generation/${this.session.id}/import`, data);
      this.$router.go();
    },
    selectFile(event) {
      this.generationState = event.target.files[0];
    },
  },

  data() {
    return {
      session: {},
      generations: [],
      loaded: false,
      generationState: null,
    };
  },
  mounted() {
    window.axios
      .get("/api/generations/" + this.$route.params.id)
      .then((response) => {
        const data = response.data;
        this.generations = data.generations;
        this.session = data.session;

        let i = this.generations.length;
        this.generations.forEach((generation) => {
          generation.index = ordinal(i);
          i -= 1;
        });
        this.loaded = true;
      });
  },
  components: {
    ItemNotFound,
    Navigation,
  },
  props: ["selection"],
};
</script>